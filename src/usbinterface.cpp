#include "usbinterface.h"

USBInterface::USBInterface()
{
    int rtnValue;
    rtnValue = libusb_init(&m_USBSession);

    if(rtnValue < 0)
    {
        qWarning() << "Failed to init libusb: " << rtnValue;
    }
    else
    {
        libusb_set_option(m_USBSession, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);
//        libusb_set_option(m_USBSession, LIBUSB_OPTION_USE_USBDK);
        init_device();
    }
}

USBInterface::~USBInterface()
{
    if(m_USBSession != NULL)
    {
        libusb_exit(m_USBSession);
    }
}

bool USBInterface::writeRegister(Register *wrReg)
{
    return false;
}

bool USBInterface::writeSequence(const std::vector<Register *> &wrSequence)
{
    return false;
}

const QString &USBInterface::interfaceName() const
{
    return m_interfaceName;
}

bool USBInterface::init_device()
{
    libusb_device **devices_list_ptr;
    ssize_t devices_list_size;

    devices_list_size = libusb_get_device_list(m_USBSession, &devices_list_ptr);
    if(devices_list_size < 0)
    {
        qWarning() << "libusb get device list error";
        return false;
    }

    qInfo() << "Devices in list: " << devices_list_size;
    for(int i = 0; i < devices_list_size; i++)
    {
        qInfo() << deviceInfo(devices_list_ptr[i]); //print specs of this device
    }

    libusb_free_device_list(devices_list_ptr, 1);
    return true;
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
        deviceInfo << "Number of possible configurations: " + QString::number(desc.bNumConfigurations);


//        libusb_device_handle* dev_handle;
//        int rtnVal = libusb_open(dev, &dev_handle);
//        if(rtnVal == LIBUSB_SUCCESS)
//        {
//            unsigned char product[200];
//            libusb_get_string_descriptor_ascii(dev_handle, desc.iProduct, product,200);
//            qWarning() << QString::fromUtf8(reinterpret_cast<const char*>(product));
//            libusb_close(dev_handle);
//        }
//        else
//        {
//            qWarning() << "Getting dev_handle failed: " << libusb_strerror(rtnVal);
//        }

        libusb_config_descriptor *config;

        libusb_get_config_descriptor(dev, 0, &config);
        deviceInfo << "Interfaces: " + QString::number(config->bNumInterfaces);

//        const libusb_interface *inter;
//        const libusb_interface_descriptor *interdesc;
//        const libusb_endpoint_descriptor *epdesc;

//        for(int i=0; i<(int)config->bNumInterfaces; i++)
//        {
//            inter = &config->interface[i];
//            qInfo()<<"Number of alternate settings: "<<inter->num_altsetting;

//            for(int j=0; j<inter->num_altsetting; j++)
//            {
//                interdesc = &inter->altsetting[j];
//                qInfo()<<"Interface Number: "<<(int)interdesc->bInterfaceNumber;
//                qInfo()<<"Number of endpoints: "<<(int)interdesc->bNumEndpoints;

//                for(int k=0; k<(int)interdesc->bNumEndpoints; k++)
//                {
//                    epdesc = &interdesc->endpoint[k];
//                    qInfo()<<"Descriptor Type: "<<(int)epdesc->bDescriptorType;
//                    qInfo()<<"EP Address: "<<(int)epdesc->bEndpointAddress;
//                }
//            }
//        }

        libusb_free_config_descriptor(config);
        return deviceInfo;
}
