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

//        rtnValue = libusb_hotplug_register_callback(m_USBSession, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED |
//                                                    LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, 0, m_VID, m_PID,
//                                                    LIBUSB_HOTPLUG_MATCH_ANY, hotplugCallback, NULL,
//                                                    &callbackHandle);

//        qInfo() << libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG);
//        if (rtnValue != LIBUSB_SUCCESS)
//        {
//            qWarning() << "Failed to register callback:" << libusb_error_name(rtnValue);
//        }
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

    return false;
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

    for(auto it = wrSequence.begin(); it != wrSequence.end(); ++it)
    {
        // лучше собрать большой QByteArray из всех регистров, преобразовать
        // и отослать за раз bulk transfer. В начале пакета или перед каждым регистром указывать
        // режим - SPI, MSB/LSB, размер регистра(в битах, но отсылка с заполнением до байта),
        // размер в посылке в байтах
        char* rawData = (*it)->rawData().data();
        int dataSize = (*it)->rawData().size();

        rtnValue = libusb_bulk_transfer(m_activeDevice.handle,
                                        (m_activeDevice.endpointAddress | LIBUSB_ENDPOINT_OUT), // write
                                        (unsigned char*)rawData,
                                        dataSize,
                                        &actualWrittenBytes,
                                        m_timeout);

        if(rtnValue < 0)
        {
            qWarning() << "Failed send to USB device:" << libusb_error_name(rtnValue);
            return false;
        }
        if(dataSize != actualWrittenBytes)
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
        qWarning() << "Can't open flash";
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

        device.deviceName = QString::fromUtf8(reinterpret_cast<const char*>(manufacturer)).simplified() +
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

//int USBInterface::hotplugCallback(libusb_context *context, libusb_device *device, libusb_hotplug_event event, void *userData)
//{
//    static libusb_device_handle *dev_handle = NULL;
//    struct libusb_device_descriptor desc;
//    int rtnValue;

//    (void)libusb_get_device_descriptor(device, &desc);

//    if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED)
//    {
//      rtnValue = libusb_open(device, &dev_handle);
//      if (rtnValue != LIBUSB_SUCCESS)
//      {
//        qWarning() << "Could not open USB device";
//      }
//      qInfo() << "connected";
//    }
//    else if (LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT == event)
//    {
//      if (dev_handle)
//      {
//        libusb_close(dev_handle);
//        dev_handle = NULL;
//      }
//      qInfo() << "disconnected";
//    }
//    else
//    {
//      qWarning() << "Unhandled event: " << event;
//    }

//    return 0;
//}

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
            deviceInfo << "Number of alternate settings: " + QString::number(inter->num_altsetting);

            for(int j=0; j<inter->num_altsetting; j++)
            {
                interdesc = &inter->altsetting[j];
                deviceInfo << "Interface Number: " + QString::number(interdesc->bInterfaceNumber);
                deviceInfo << "Number of endpoints: " + QString::number(interdesc->bNumEndpoints);

                for(int k=0; k<(int)interdesc->bNumEndpoints; k++)
                {
                    epdesc = &interdesc->endpoint[k];
                    deviceInfo << "Descriptor Type: " + QString::number(epdesc->bDescriptorType);
                    deviceInfo << "EP Address: " + QString::number(epdesc->bEndpointAddress);
                }
            }
        }

        libusb_free_config_descriptor(config);
        return deviceInfo;
}

}
