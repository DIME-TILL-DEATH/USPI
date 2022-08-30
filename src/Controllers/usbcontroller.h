#ifndef USBCONTROLLER_H
#define USBCONTROLLER_H

#include "libusb.h"

#include "abstractinterface.h"
#include "abstractcontroller.h"


class USBController : public AbstractController
{
public:
    USBController(const QString &name, const QMap<QString, DevicePin> &devicePins = {});

    // private????
    // TODO: rearrange
    libusb_device* device_ptr{nullptr};
    libusb_device_handle* handle{nullptr};
    libusb_device_descriptor deviceDescriptor;

    int interfaceNumber{0};
    unsigned char endpointAddress{4};

    quint16 deviceBufferSize{256};

    QStringList deviceInfo;
    // TODO: use m_deviceName
    QString deviceName;

    QStringList avaliableChannels() override;
private:

};

#endif // USBCONTROLLER_H
