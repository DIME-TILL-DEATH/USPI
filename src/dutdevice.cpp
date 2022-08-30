#include "dutdevice.h"
#include "JsonWorker.h"

DUTDevice::DUTDevice()
{
    m_deviceHeader.device_ptr = this;
}

bool DUTDevice::loadFromFile(const QString &fileName, ParseError *error)
{
    JsonWorker jsonFile;

    if(!jsonFile.loadFile(fileName, error)) return false;

    if(!loadFromJsonObject(jsonFile.deviceGlobalObject(), error)) return false;
    return true;
}

bool DUTDevice::loadFromJsonObject(const QJsonObject &jsonObject, ParseError *error)
{
    JsonWorker worker(jsonObject);

    if(!worker.readHeader(&m_deviceHeader, error)) return false;

    m_deviceHeader.deviceType = DeviceType::DUT;

    if(!worker.readRegisterArray(&m_deviceRegisterMap, &m_deviceHeader, error)) return false;

    return true;
}

void DUTDevice::registerTypes()
{
    qmlRegisterUncreatableType<DUTHeader>("DUTDevice", 1, 0, "DeviceHeader", "DUT header");
}

const QString &DUTDevice::name() const
{
    return m_deviceHeader.deviceName;
}

std::vector<std::shared_ptr<Register> > &DUTDevice::deviceRegisterMap()
{
    return m_deviceRegisterMap;
}

const DUTHeader &DUTDevice::deviceHeader() const
{
    return m_deviceHeader;
}

void DUTDevice::setChannelNumber(qint8 chNum)
{
    m_deviceHeader.channelNumber = chNum;
}

qint8 DUTDevice::channelNumber()
{
    return m_deviceHeader.channelNumber;
}

std::shared_ptr<Register> DUTDevice::registerByUniqueId(quint16 uniqueId)
{
    for(auto it = m_deviceRegisterMap.begin(); it != m_deviceRegisterMap.end(); ++it)
    {
        if((*it)->uniqueId() == uniqueId) return (*it);
    }
    qWarning() << QObject::tr("Регистр с UniqueId ") << uniqueId << QObject::tr(" не найден!");
    return nullptr;
}

std::shared_ptr<Register> DUTDevice::registerByName(QString registerName)
{
    for(auto it = m_deviceRegisterMap.begin(); it != m_deviceRegisterMap.end(); ++it)
    {
        if((*it)->name() == registerName) return (*it);
    }
    qWarning() << QObject::tr("Регистр ") << registerName << (" не найден!");
    return nullptr;
}

AbstractField *DUTDevice::findField(QString registerName, QString fieldName)
{
    auto findedReg = std::find_if(m_deviceRegisterMap.begin(), m_deviceRegisterMap.end(), [registerName](std::shared_ptr<Register> reg)
                                                                            {return reg->name() == registerName;});
    if(findedReg == m_deviceRegisterMap.end())
    {
        qWarning() << QObject::tr("Регистр с именем ") << registerName << QObject::tr(" не найден в карте регистров устройства");
        return nullptr;
    }

    AbstractField* findedField = (*findedReg)->field(fieldName);

    return findedField;
}

QDataStream& operator<<(QDataStream& stream, const DUTDevice& device)
{
    stream.setVersion(QDataStream::Qt_5_15);

    stream << device.m_deviceHeader.version;
    stream << device.m_deviceHeader.deviceName;
    stream << device.m_deviceHeader.registerSize;
    stream << device.m_deviceHeader.isMSB;

    size_t registerMapSize = device.m_deviceRegisterMap.size();
    stream << registerMapSize;
    for(auto it = device.m_deviceRegisterMap.begin();
            it != device.m_deviceRegisterMap.end();
            ++it)
    {
        stream << (*(*it));
    }
    return stream;
}

QDataStream& operator>>(QDataStream& stream, DUTDevice& device)
{
    stream.setVersion(QDataStream::Qt_5_15);

    stream >> device.m_deviceHeader.version;
    stream >> device.m_deviceHeader.deviceName;
    stream >> device.m_deviceHeader.registerSize;
    stream >> device.m_deviceHeader.isMSB;

    size_t registerMapSize;
    stream >> registerMapSize;

    device.m_deviceRegisterMap.clear();
    for(quint16 i=0; i < registerMapSize; i++)
    {
        Register* readedRegister_ptr = new Register;
        stream >> *readedRegister_ptr;
        device.m_deviceRegisterMap.push_back(std::shared_ptr<Register>(readedRegister_ptr));
    }
    return stream;
}
