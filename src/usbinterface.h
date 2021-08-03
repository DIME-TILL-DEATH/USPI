#ifndef USBINTERFACE_H
#define USBINTERFACE_H

#include <QDebug>

#include "abstractinterface.h"
#include "libusb.h"

class USBInterface : public AbstractInterface
{
public:
    USBInterface();
    ~USBInterface();

    bool writeRegister(Register* wrReg) override;
    bool writeSequence(const std::vector<Register*> &wrSequence) override;

    const QString &interfaceName() const override;

private:
    QString m_interfaceName{"USB"};

    libusb_context* m_USBSession {NULL};

    int m_VID{0};
    int m_PID{0};

    bool init_device();
    QStringList deviceInfo(libusb_device *dev);
};

#endif // USBINTERFACE_H
