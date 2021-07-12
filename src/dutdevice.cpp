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

quint16 DUTDevice::registerCount()
{
    return m_deviceRegisterMap.size();
}

const Register &DUTDevice::registerAt(quint16 registerIndex) const
{
    return m_deviceRegisterMap.at(registerIndex);
}
