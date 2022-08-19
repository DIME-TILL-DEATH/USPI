#ifndef USBINTERFACE_H
#define USBINTERFACE_H

#include <QDebug>

#include "libusb.h"

#include "abstractinterface.h"
#include "interfacenames.h"

#include "usbcontroller.h"

namespace USBBitPosition
{
//    const quint8 SPI{7};
//    const quint8 PARALLEL{6};
    const quint8 CHANNEL{4};
    const quint8 ORDER{5};
    const quint8 TRIGGER{4};
}

namespace USBFieldSize
{
    const quint8 HEADER{4};
}

class USBInterface : public AbstractInterface
{
public:
    USBInterface(QObject* parent=nullptr);
    ~USBInterface();

    /** Write data to register */
    bool writeRegister(Register* wrReg) override;
    bool writeSequence(const std::vector<Register*> &wrSequence) override;

    const QString &interfaceName() const override;

    //std::vector<std::shared_ptr<AbstractController> >& connectedControllers() override;
    const USBController &activeController() const;

    void refreshUSBDevices();

private:
    QString m_interfaceName;

    libusb_context* m_USBSession {nullptr};

    // путаница с контроллерами в AbstractController. переписать чтоб использовался оттуда.
    // по месту делать dynamic_cast если нужны специфичные пункты
    // TODO:
    // пока так, только один тип устройств
    // потом добавить функцию проверки соответствует ли устройство в списке полученных устройств USB
    // поддерживаемому типу, если да, открывать и сохранять указатели в list/vector и далее
    // позволять пользователю выбирать активное устройство (shared_ptr!). Отправлять через него.
    // в closeDevice закрывать все
    USBController m_activeController;

    // TODO:
    // перенести в usbcontroller
    int m_VID{0x03EB}; //0x03EB - Atmel //0x04D8 - microchip
    int m_PID{0x204F}; //0x2FF0 - ATMega32U2 DFU

    int m_timeout{30000};

    char* m_rawData{nullptr};


    bool initUSB();
    bool initDevice(USBController &device);
    void closeDevice(USBController &device);


    QStringList deviceInfo(libusb_device *dev);
    QString epTypeString(const libusb_endpoint_descriptor& epDescriptor);
    QString deviceSpeedString(libusb_device *dev);

    QByteArray formHeader(quint16 regCount, quint16 packetSize);
};


#endif // USBINTERFACE_H
