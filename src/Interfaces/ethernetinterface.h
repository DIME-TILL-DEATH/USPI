#ifndef ETHERNETINTERFACE_H
#define ETHERNETINTERFACE_H

#include <QUdpSocket> // возможно стои сделать на TCP/IP
#include <QHostAddress>
#include <QNetworkInterface>

#include "abstractinterface.h"

namespace EthernetBitPosition
{
    const quint8 TRIGGER{4};
}

namespace EthernetConstants
{
    const quint8 ProtocolVersion{1};
    const quint8 HeaderSize{6};
}

namespace EthernetFrames
{
    const quint8 SendData{0x1};
    const quint8 RequestDeviceConfiguration{0x2};
    const quint8 SendAcknowledge{0x8};
    const quint8 DeviceConfiguration{0x9};
    const quint8 DeviceMalfunction{0xF};
}



class EthernetInterface : public AbstractInterface
{
public:
    EthernetInterface(QObject* parent=nullptr);

    bool writeRegister(Register *wrReg);
    bool writeSequence(const std::vector<Register *> &wrSequence);

    const QString &interfaceName() const;

//    std::vector<std::shared_ptr<AbstractController> > &connectedControllers();

private slots:
    void slReadyRead();

private:
    QString m_interfaceName;

    QUdpSocket* m_udpSocket;

    QByteArray formHeader(quint16 regCount, quint16 packetSize);
};

#endif // ETHERNETINTERFACE_H
