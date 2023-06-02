#include "usbinterface.h"



USBInterface::USBInterface(QObject *parent)
     :AbstractInterface(parent),
      m_interfaceName{InterfaceNames::USB},
      m_activeController{"null", {}}
{
    int rtnValue;
    rtnValue = libusb_init(&m_USBSession);

    m_isAvaliable = false;

    if(rtnValue < 0)
    {
        qWarning() << tr("Не удалось инициализировать libusb: ") << libusb_error_name(rtnValue);
    }
    else
    {
        libusb_set_option(m_USBSession, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);
        m_rawData = static_cast<unsigned char*>(std::malloc(m_activeController.deviceBufferSize));
    }
}

USBInterface::~USBInterface()
{
    if(m_USBSession != nullptr)
    {
        closeDevice(m_activeController);
        libusb_exit(m_USBSession);
       // std::free(m_rawData);
    }
}

bool USBInterface::writeRegister(Register *wrReg)
{
    // Warn! Method doesn't tested!!!
//    QByteArray regData = wrReg->rawData();

//    char* rawData = regData.data();
//    int dataSize = regData.size();

//    int actualWrittenBytes;
//    int rtnValue = libusb_bulk_transfer(m_activeController.handle,
//                                    m_activeController.endpointAddress | LIBUSB_ENDPOINT_OUT,
//                                    (unsigned char*)rawData,
//                                    dataSize,
//                                    &actualWrittenBytes,
//                                    m_timeout);

//    if(rtnValue < 0)
//    {
//        qWarning() << "Failed send to USB device:" << libusb_error_name(rtnValue);
//        return false;
//    }
//    if(dataSize > actualWrittenBytes)
//    {
//        qWarning() << "Sended: " << actualWrittenBytes << ", while data size is: " << dataSize;
//        return false;
//    }
    return true;
}

bool USBInterface::writeSequence(const std::vector<Register *> &wrSequence)
{
    if(m_activeController.handle == nullptr)
    {
        qWarning() << tr("Устройство USB не готово");
        return false;
    }

    bool isPacketFragmented = false;
    quint8 fragmentCount = 0;

    QByteArray sequenceData;
    quint16 bytesInPacket = 0;
    quint16 regInPacket = 0;

    for(auto itSeq = wrSequence.begin(); itSeq != wrSequence.end(); ++itSeq)
    {
        //QByteArray regData = (*itSeq)->rawData();

        QList<QByteArray> rawDataList = (*itSeq)->rawData();

        if((*itSeq)->parentDUTHeader()->isMSB)
        {
            std::reverse(rawDataList.begin(), rawDataList.end());
        }

        for(auto itReg = rawDataList.begin(); itReg != rawDataList.end(); ++itReg)
        {
            QByteArray regData = (*itReg);
            quint16 bitSize = regData.size()*8;

            if((USBFieldSize::HEADER+sequenceData.size()+regData.size()+USBFieldSize::REGISTER)>256) // фрагментированная отсылка
            {
                isPacketFragmented = true;

                sequenceData.prepend(formHeader(regInPacket, bytesInPacket, isPacketFragmented, fragmentCount));
                m_rawData = (unsigned char*)sequenceData.data();
                if(!sendPacket(m_rawData, sequenceData.size())) return false;

                sequenceData.clear();
                bytesInPacket=0;
                regInPacket=0;
                fragmentCount++;

                // иначе USB теряет пакет
                QThread::msleep(200);
            }

            if((*itSeq)->parentDUTHeader()->isMSB)
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

            regData.prepend((*itSeq)->parentDUTHeader()->channelNumber<<USBBitPosition::CHANNEL | (uchar)(*itSeq)->parentDUTHeader()->deviceType);
            regData.prepend(bitSize);

            bytesInPacket += regData.size();
            regInPacket++;

            sequenceData.append(regData);
        }
    }

    sequenceData.prepend(formHeader(regInPacket, bytesInPacket, isPacketFragmented, fragmentCount));
    m_rawData = (unsigned char*)sequenceData.data();
    bool result = sendPacket(m_rawData, sequenceData.size());

    return result;
}

bool USBInterface::sendPacket(unsigned char* data_ptr, quint16 dataSize)
{
    int actualWrittenBytes = 0;
    int rtnValue = 0;

    rtnValue = libusb_bulk_transfer(m_activeController.handle,
                                    m_activeController.endpointAddress | LIBUSB_ENDPOINT_OUT,
                                    data_ptr,//(unsigned char*)data_ptr,
                                    dataSize,
                                    &actualWrittenBytes,
                                    m_timeout);
    if(rtnValue < 0)
    {
        qWarning() << tr("Ошибка отправки на USB устройство:") << libusb_strerror(rtnValue);
        return false;
    }
    if(dataSize > actualWrittenBytes)
    {
        qWarning() << tr("Отправлено: ") << actualWrittenBytes << tr(", тогда как размер посылки: ") << dataSize;
        return false;
    }
    qInfo() << tr("Отправлено: ") << actualWrittenBytes << tr(" байт");
    return true;
}

const QString &USBInterface::interfaceName() const
{
    return m_interfaceName;
}

//std::vector<std::shared_ptr<AbstractController> > &USBInterface::connectedControllers()
//{

//}

const USBController &USBInterface::activeController() const
{
    return m_activeController;
}

void USBInterface::refreshUSBDevices()
{
    closeDevice(m_activeController);
    initUSB();
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
    m_isAvaliable = false;
    m_activeController.handle = libusb_open_device_with_vid_pid(m_USBSession, m_VID, m_PID);

//    libusb_free_device_list(devices_list_ptr, 1);

    if(m_activeController.handle == nullptr)
    {
        m_activeController.deviceName.clear();
        m_activeController.deviceInfo.clear();

        qWarning() << tr("Не удалось открыть USB устройство.");
        return false;
    }

    if(!initDevice(m_activeController))
    {
        qWarning() << tr("Не удалось инициализировать USB устройство.");
        return false;
    }

//    qInfo() << m_activeDevice.deviceName;

    m_isAvaliable = true;
    return true;
}

bool USBInterface::initDevice(USBController& device)
{
        libusb_set_auto_detach_kernel_driver(device.handle, 1);

        int rtnValue = libusb_claim_interface(device.handle, device.interfaceNumber);

        if(rtnValue < 0)
        {
            qWarning() << tr("Failed to claim interface: ") << libusb_strerror(rtnValue);
            return false;
        }


        device.device_ptr = libusb_get_device(device.handle);

        rtnValue = libusb_get_device_descriptor(device.device_ptr, &device.deviceDescriptor);

        if (rtnValue < 0)
        {
            qWarning() << tr("Failed to get device descriptor: ") << libusb_strerror(rtnValue);
            return false;
        }
        unsigned char manufacturer[200];
        unsigned char product[200];

        // TODO: use USBController methods to get info about device

        libusb_get_string_descriptor_ascii(device.handle, device.deviceDescriptor.iProduct, product, 200);
        libusb_get_string_descriptor_ascii(device.handle, device.deviceDescriptor.iManufacturer, manufacturer, 200);

        device.deviceName = QString::fromLatin1(reinterpret_cast<const char*>(manufacturer)).simplified() +
                        "|" +
                       QString::fromUtf8(reinterpret_cast<const char*>(product)).simplified();

        device.deviceInfo = deviceInfo(device.device_ptr);


        return true;
}

void USBInterface::closeDevice(USBController& device)
{
    if(device.handle != nullptr)
    {
        libusb_release_interface(device.handle, device.interfaceNumber);
        libusb_close(device.handle);

        device.handle = nullptr;
//        device.deviceInfo.clear();
//        device.deviceName.clear();
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

QByteArray USBInterface::formHeader(quint16 regCount, quint16 packetSize, bool isFragmented, quint8 fragmentNumber)
{
    //uchar reservedByte = 0x00;

    QByteArray result;

    result.append(0<<USBBitPosition::TRIGGER);// | (m_deviceHeader.channelNumber & 0xF));
    result.append(regCount);
    result.append(packetSize + USBFieldSize::HEADER);
    result.append(((fragmentNumber&0x3)<<USBBitPosition::FRAGMENT_COUNT) | (isFragmented&0x1));

    return result;
}

