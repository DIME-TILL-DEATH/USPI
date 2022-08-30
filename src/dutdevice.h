#ifndef DUTDEVICE_H
#define DUTDEVICE_H

#include <QtQml>

#include <QString>
#include <QDataStream>

#include <vector>
#include <memory>

#include "parseerror.h"
#include "register.h"
#include "registeradapter.h"

class JsonWorker;

enum class DeviceType
{
    DUT=0x00,
    Controller=0x01
};

struct DUTHeader
{
private:
        Q_GADGET

        Q_PROPERTY(QString deviceName MEMBER deviceName);
        Q_PROPERTY(QString description MEMBER description);
        Q_PROPERTY(quint8 registerSize MEMBER registerSize);
        Q_PROPERTY(bool isMSB MEMBER isMSB);
        Q_PROPERTY(quint8 channelNumber MEMBER channelNumber);
public:

    QString deviceName{QObject::tr("Выбрать")};
    QString description;
    QString version{""};
    quint8 registerSize{32};
    bool isMSB{true};
    quint8 channelNumber{0};
    qint16 uniqueId;

    DeviceType deviceType;
    DUTDevice* device_ptr;
};
Q_DECLARE_METATYPE(DUTHeader)

class DUTDevice
{
public:
    friend class JsonWorker;

    DUTDevice();

    bool loadFromFile(const QString& fileName, ParseError* error = nullptr);
    bool loadFromJsonObject(const QJsonObject& jsonObject, ParseError* error = nullptr);

    static void registerTypes();

    const QString& name() const;

    std::vector<std::shared_ptr<Register> > &deviceRegisterMap();
    const DUTHeader &deviceHeader() const;

    void setChannelNumber(qint8 chNum);
    qint8 channelNumber();

    std::shared_ptr<Register> registerByUniqueId(quint16 uniqueId);
    std::shared_ptr<Register> registerByName(QString registerName);

    AbstractField* findField(QString registerName, QString fieldName);

    friend QDataStream& operator<<(QDataStream& stream, const DUTDevice& device);
    friend QDataStream& operator>>(QDataStream& stream, DUTDevice& device);
private:
    DUTHeader m_deviceHeader;

    std::vector<std::shared_ptr<Register> > m_deviceRegisterMap{};
};


#endif // DUTDEVICE_H
