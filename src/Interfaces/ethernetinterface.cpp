#include "ethernetinterface.h"

#include "interfacenames.h"

EthernetInterface::EthernetInterface(QObject *parent)
                  :AbstractInterface(parent),
                   m_interfaceName{InterfaceNames::Ethernet}
{
    m_udpSocket = new QUdpSocket(this);

//    foreach(const QHostAddress &laddr, QNetworkInterface::allAddresses())
//     {
//         qDebug() << "Found IP:" << laddr.toString();
//     }

    bool result = m_udpSocket->bind(QHostAddress("192.168.104.1"));
    if(!result) qWarning() << m_udpSocket->errorString();

    connect(m_udpSocket, &QUdpSocket::readyRead, this, &EthernetInterface::slReadyRead);
}


bool EthernetInterface::writeRegister(Register *wrReg)
{
    return false;
}

bool EthernetInterface::writeSequence(const std::vector<Register *> &wrSequence)
{
    QByteArray sequenceData;
    quint16 bytesInPacket = 0;
    quint16 regInPacket = 0;

    for(auto itSeq = wrSequence.begin(); itSeq != wrSequence.end(); ++itSeq)
    {
        QList<QByteArray> rawDataList = (*itSeq)->rawData();

        if((*itSeq)->parentDUTHeader()->isMSB)
        {
            std::reverse(rawDataList.begin(), rawDataList.end());
        }

        for(auto itReg = rawDataList.begin(); itReg != rawDataList.end(); ++itReg)
        {
            QByteArray regData = (*itReg);
            quint16 bitSize = regData.size()*8;

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

            regData.prepend((uchar)(*itSeq)->parentDUTHeader()->deviceType);
            regData.prepend(bitSize);

            bytesInPacket += regData.size();
            regInPacket++;

            sequenceData.append(regData);
        }
    }

    QByteArray header = formHeader(regInPacket, bytesInPacket);
    sequenceData.prepend(header);

    m_udpSocket->writeDatagram(sequenceData, QHostAddress("192.168.104.10"), 15600);

    return true;
}

const QString &EthernetInterface::interfaceName() const
{
    return m_interfaceName;
}

//std::vector<std::shared_ptr<AbstractController> > &EthernetInterface::connectedControllers()
//{

//}

void EthernetInterface::slReadyRead()
{
    // when data comes in
    QByteArray buffer;
    buffer.resize(m_udpSocket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;

    m_udpSocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

    qInfo() << "Message from: " << sender.toString() << " Message port: " << senderPort << " Message: " << buffer;
}

QByteArray EthernetInterface::formHeader(quint16 regCount, quint16 packetSize)
{
    uchar reservedByte = 0x00;

    QByteArray result;

    result.append(EthernetFrames::SendData);
    result.append(EthernetConstants::ProtocolVersion);
    result.append(0<<EthernetBitPosition::TRIGGER); // | (m_deviceHeader.channelNumber & 0xF));
    result.append(regCount);
    result.append(packetSize + EthernetConstants::HeaderSize);
    result.append(reservedByte);

    return result;
}
