#include "dutdevice.h"
#include "fileparser.h"

bool DUTDevice::loadFromFile(const QString &fileName, ParseError *error)
{
    FileParser jsonFile;

    if(!jsonFile.loadFile(fileName, error)) return false;
    if(!jsonFile.readHeader(&m_deviceHeader, error)) return false;
    if(!jsonFile.readRegisterArray(&m_deviceRegisterMap, error)) return false;

    for(auto it=m_deviceRegisterMap.begin(); it != m_deviceRegisterMap.end(); ++it)
    {
        if(!(*it)->sortAndValidateFields(error))
        {
            ParseError fieldError;
            fieldError.setErrorType(ParseError::ErrorType::RegisterContentError,
                                    " '" +(*it)->name() + "', "+
                                    error->errorString());
            *error = fieldError;
            return false;
        }
    }
    return true;
}

const QString &DUTDevice::name() const
{
    return m_deviceHeader.deviceName;
}

std::vector<std::shared_ptr<Register> > &DUTDevice::deviceRegisterMap()
{
    return m_deviceRegisterMap;
}

const DUTDevice::Header &DUTDevice::deviceHeader() const
{
    return m_deviceHeader;
}

std::shared_ptr<Register> DUTDevice::registerByUniqueId(quint16 uniqueId)
{
    for(auto it = m_deviceRegisterMap.begin(); it != m_deviceRegisterMap.end(); ++it)
    {
        if((*it)->uniqueId() == uniqueId) return (*it);
    }
    return nullptr;
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
