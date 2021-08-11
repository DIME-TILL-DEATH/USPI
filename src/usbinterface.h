#ifndef USBINTERFACE_H
#define USBINTERFACE_H

#include <QDebug>

#include "abstractinterface.h"
#include "libusb.h"

namespace USB
{

static QString interfaceNameString = "USB";

struct USBDevice{
    libusb_device* device_ptr{nullptr};
    libusb_device_handle* handle{nullptr};
    libusb_device_descriptor deviceDescriptor;

    int interfaceNumber{0};
    unsigned char endpointAddress{2};

    QString deviceName;
    QStringList deviceInfo;
};

class USBInterface : public AbstractInterface
{
public:
    USBInterface();
    ~USBInterface();

    /** Write data to register */
    bool writeRegister(Register* wrReg) override;
    bool writeSequence(const std::vector<Register*> &wrSequence) override;

    const QString &interfaceName() const override;

    const USBDevice &activeDevice() const;

private:
    QString m_interfaceName;

    libusb_context* m_USBSession {nullptr};

    USBDevice m_activeDevice;
    // TODO:
    // пока так, только один тип устройств
    // потом добавить функцию проверки соответствует ли устройство в списке полученных устройств USB
    // поддерживаемому типу, если да, открывать и сохранять указатели в list/vector и далее
    // позволять пользователю выбирать активное устройство (m_activeDevice_ptr или итератор). Отправлять через него.
    // не забывать что ссылки на элемент вектора протухают при изменении содержания!!!
    int m_VID{0x1A86}; //0x2109 - billboard //0x04D8 - microchip
    int m_PID{0xE008}; //0x0102 - billboard

    int m_timeout{1000};


    bool initUSB();
    bool initDevice(USBDevice& device);
    void closeDevice(USBDevice& device);


    QStringList deviceInfo(libusb_device *dev);
    QString epTypeString(const libusb_endpoint_descriptor& epDescriptor);
};
}

#endif // USBINTERFACE_H
