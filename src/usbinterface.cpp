#include "usbinterface.h"

namespace USB {

USBInterface::USBInterface()
    :m_interfaceName{interfaceNameString}
{
    int rtnValue;
    rtnValue = libusb_init(&m_USBSession);
    isActive = false;

    if(rtnValue < 0)
    {
        qWarning() << "Failed to init libusb: " << libusb_error_name(rtnValue);
    }
    else
    {
        libusb_set_option(m_USBSession, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);

        initUSB();
    }
}

USBInterface::~USBInterface()
{
    if(m_USBSession != nullptr)
    {
        closeDevice(m_activeDevice);
        libusb_exit(m_USBSession);
    }
}

bool USBInterface::writeRegister(Register *wrReg)
{
    // Warn! Method doesn't tested!!!
    QByteArray regData = wrReg->rawData();

    char* rawData = regData.data();
    int dataSize = regData.size();

    int actualWrittenBytes;
    int rtnValue = libusb_bulk_transfer(m_activeDevice.handle,
                                    m_activeDevice.endpointAddress | LIBUSB_ENDPOINT_OUT,
                                    (unsigned char*)rawData,
                                    dataSize,
                                    &actualWrittenBytes,
                                    m_timeout);

    if(rtnValue < 0)
    {
        qWarning() << "Failed send to USB device:" << libusb_error_name(rtnValue);
        return false;
    }
    if(dataSize > actualWrittenBytes)
    {
        qWarning() << "Sended: " << actualWrittenBytes << ", while data size is: " << dataSize;
        return false;
    }
    return true;
}

bool USBInterface::writeSequence(const std::vector<Register *> &wrSequence)
{
    if(m_activeDevice.handle == nullptr)
    {
        qWarning() << "USB device not ready";
        return false;
    }

    int actualWrittenBytes = 0;
    int rtnValue = 0;

    QByteArray header = formHeader(wrSequence);

    rtnValue = libusb_bulk_transfer(m_activeDevice.handle,
                                    m_activeDevice.endpointAddress | LIBUSB_ENDPOINT_OUT,
                                    (unsigned char*)header.data(),
                                    header.size(),
                                    &actualWrittenBytes,
                                    m_timeout);
    if(rtnValue < 0)
    {
        qWarning() << "Failed send to USB device:" << libusb_strerror(rtnValue);
        return false;
    }

    for(auto it = wrSequence.begin(); it != wrSequence.end(); ++it)
    {
        QByteArray regData;

        regData = (*it)->rawData();

        if(m_deviceHeader.isMSB)
        {
            std::reverse(regData.begin(), regData.end());
        }
        else
        {
            for(QByteArray::iterator it=regData.begin();it!=regData.end();++it)
            {
                reverseByte(*it);
            }
        }

        regData.prepend((uchar)0x00); //reserved byte
        regData.prepend((*it)->bitSize());

        char* rawData = regData.data();
        int dataSize = regData.size();

        rtnValue = libusb_bulk_transfer(m_activeDevice.handle,
                                        m_activeDevice.endpointAddress | LIBUSB_ENDPOINT_OUT,
                                        (unsigned char*)rawData,
                                        dataSize,
                                        &actualWrittenBytes,
                                        m_timeout);

        if(rtnValue < 0)
        {
            qWarning() << "Failed send to USB device:" << libusb_strerror(rtnValue);
            return false;
        }
        if(dataSize > actualWrittenBytes)
        {
            qWarning() << "Sended: " << actualWrittenBytes << ", while data size is: " << dataSize;
            return false;
        }
    }

    return true;
}

const QString &USBInterface::interfaceName() const
{
    return m_interfaceName;
}

const USBDevice& USBInterface::activeDevice() const
{
    return m_activeDevice;
}


bool USBInterface::initUSB()
{
    // вызывает баг "warning [libusb_exit] some libusb_devices were leaked"
    // при дальнейшем использовании класса
    // можно попробовать хранить ссылку на список устройств как член класса
    // и очищать лист в деструкторе

//    libusb_device **devices_list_ptr;
//    ssize_t devices_list_size;

//    devices_list_size = libusb_get_device_list(m_USBSession, &devices_list_ptr);
//    if(devices_list_size < 0)
//    {
//        qWarning() << "libusb get device list error";
//        return false;
//    }

    m_activeDevice.handle = libusb_open_device_with_vid_pid(m_USBSession, m_VID, m_PID);

//    libusb_free_device_list(devices_list_ptr, 1);

    if(m_activeDevice.handle == nullptr)
    {
        qWarning() << "Can't open device";
        return false;
    }

    if(!initDevice(m_activeDevice))
    {
        qWarning() << "Device initialization failed";
        return false;
    }

    qInfo() << m_activeDevice.deviceName;

    isActive = true;
    return true;
}

bool USBInterface::initDevice(USBDevice& device)
{
        libusb_set_auto_detach_kernel_driver(device.handle, 1);

        int rtnValue = libusb_claim_interface(device.handle, device.interfaceNumber);

        if(rtnValue < 0)
        {
            qWarning() << "Failed to claim interface: " << libusb_strerror(rtnValue);
            return false;
        }


        device.device_ptr = libusb_get_device(device.handle);

        rtnValue = libusb_get_device_descriptor(device.device_ptr, &device.deviceDescriptor);

        if (rtnValue < 0)
        {
            qWarning() << "Failed to get device descriptor: " << libusb_strerror(rtnValue);
            return false;
        }
        unsigned char manufacturer[200];
        unsigned char product[200];

        libusb_get_string_descriptor_ascii(device.handle, device.deviceDescriptor.iProduct, product, 200);
        libusb_get_string_descriptor_ascii(device.handle, device.deviceDescriptor.iManufacturer, manufacturer, 200);

        device.deviceName = QString::fromLatin1(reinterpret_cast<const char*>(manufacturer)).simplified() +
                        "|" +
                       QString::fromUtf8(reinterpret_cast<const char*>(product)).simplified();

        device.deviceInfo = deviceInfo(device.device_ptr);


        return true;
}

void USBInterface::closeDevice(USBDevice& device)
{
    if(device.handle != nullptr)
    {
        libusb_release_interface(device.handle, device.interfaceNumber);
        libusb_close(device.handle);
        device.handle = nullptr;
    }
}

QStringList USBInterface::deviceInfo(libusb_device *dev)
{
        QStringList deviceInfo;

        libusb_device_descriptor desc;

        int r = libusb_get_device_descriptor(dev, &desc);

        if (r < 0)
        {
            return QStringList("Failed to get device descriptor");
        }

        deviceInfo << "VendorID: 0x" + QString::number(desc.idVendor, 16);
        deviceInfo << "ProductID: 0x" + QString::number(desc.idProduct, 16);
        deviceInfo << "Device Class: " + QString::number(desc.bDeviceClass);
        deviceInfo << "Connection speed: " + deviceSpeedString(dev);
        deviceInfo << "Number of possible configurations: " + QString::number(desc.bNumConfigurations);


        libusb_config_descriptor *config;

        libusb_get_config_descriptor(dev, 0, &config);
        deviceInfo << "Interfaces: " + QString::number(config->bNumInterfaces);

        const libusb_interface *inter;
        const libusb_interface_descriptor *interdesc;
        const libusb_endpoint_descriptor *epdesc;

        for(int i=0; i<(int)config->bNumInterfaces; i++)
        {
            inter = &config->interface[i];
            deviceInfo << "Number of alternate settings: " + QString::number(inter->num_altsetting) +"\n";

            for(int j=0; j<inter->num_altsetting; j++)
            {
                interdesc = &inter->altsetting[j];
                deviceInfo << "Interface Number: " + QString::number(interdesc->bInterfaceNumber);
                deviceInfo << "Number of endpoints: " + QString::number(interdesc->bNumEndpoints) + "\n";

                for(int k=0; k<(int)interdesc->bNumEndpoints; k++)
                {
                    epdesc = &interdesc->endpoint[k];
//                    deviceInfo << "Descriptor Type: " + QString::number(epdesc->bDescriptorType);
                    deviceInfo << "EP Address: " + QString::number(epdesc->bEndpointAddress);
                    deviceInfo << "EP type: " + epTypeString(*epdesc);
                    deviceInfo << "EP direction: " + QString((epdesc->bEndpointAddress & 0x80) ? "IN" : "OUT");
                    deviceInfo << "EP packet max size:" + QString::number(epdesc->wMaxPacketSize) + "\n";
                }
            }
        }

        libusb_free_config_descriptor(config);
        return deviceInfo;
}

QString USBInterface::epTypeString(const libusb_endpoint_descriptor& epDescriptor)
{
    switch(epDescriptor.bmAttributes & 0x3)
    {
        case LIBUSB_ENDPOINT_TRANSFER_TYPE_BULK: return "Bulk"; break;
        case LIBUSB_ENDPOINT_TRANSFER_TYPE_CONTROL: return "Control"; break;
        case LIBUSB_ENDPOINT_TRANSFER_TYPE_INTERRUPT: return "Interrupt"; break;
        case LIBUSB_ENDPOINT_TRANSFER_TYPE_ISOCHRONOUS: return "Isochronius"; break;
        default: return "Unkwnown";
    }
}

QString USBInterface::deviceSpeedString(libusb_device *dev)
{
    switch(libusb_get_device_speed(dev))
    {
        case LIBUSB_SPEED_UNKNOWN: return "Unknown"; break;
        case LIBUSB_SPEED_LOW : return " Low speed (1.5MBit/s)"; break;
        case LIBUSB_SPEED_FULL : return "Full speed (12MBit/s)"; break;
        case LIBUSB_SPEED_HIGH : return "High speed (480MBit/s)"; break;
        case LIBUSB_SPEED_SUPER : return "Super speed (5000MBit/s)"; break;
        case LIBUSB_SPEED_SUPER_PLUS  : return "Super speed plus (10000MBit/s)"; break;
        default: return "Unknown return code";
    }
}

QByteArray USBInterface::formHeader(const std::vector<Register *> &wrSequence)
{
    uchar reservedByte = 0x00;

    QByteArray result;
    result.append((1 << SPI)|(0<<PARALLEL)|(m_deviceHeader.isMSB<<ORDER)|(0<<TRIGGER));
    result.append(wrSequence.size());
    result.append(m_deviceHeader.registerSize);
    result.append(reservedByte); //reserved byte

    return result;
}

}
