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
        if(!(*it).sortAndValidateFields(error))
        {
            ParseError fieldError;
            fieldError.setErrorType(ParseError::ErrorType::RegisterContentError,
                                    " '" +(*it).name() + "', "+
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

std::vector<Register> &DUTDevice::deviceRegisterMap()
{
    return m_deviceRegisterMap;
}

const DUTDevice::Header &DUTDevice::deviceHeader() const
{
    return m_deviceHeader;
}

Register *DUTDevice::registerByUniqueId(quint16 uniqueId)
{
    // Хранить QHash/QMap в DUTDevice пока не выйдет, тк
    // много где используется получение вектора
    // регистров по неконстантной ссылке, в итоге вектор могут изменить,
    // связный список останется старым. Возможно надо переделать на возврат
    // вектора по константной ссылке, а для редактирования использовать
    // отдельные методы и в них править связный список, но не факт что это эффективнее и потянет за собой меньше ошибок
    // если этот метод не понадобится нигде кроме загрузки сессии, то можно "забить"....
    for(auto it = m_deviceRegisterMap.begin(); it != m_deviceRegisterMap.end(); ++it)
    {
        if((*it).uniqueId() == uniqueId) return &(*it);
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
        stream << (*it);
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
        Register readedRegister;
        stream >> readedRegister;
        device.m_deviceRegisterMap.push_back(readedRegister);
    }
    return stream;
}
