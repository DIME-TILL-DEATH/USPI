#include <QObject>

#include "parseerror.h"

QString ParseError::errorString()
{
    switch(m_errorType)
    {
        case ErrorType::NoError: return QObject::tr("No error");

        case ErrorType::FileError: return QObject::tr("Can't open file: ") + m_additionalErrorInfo;

        case ErrorType::PointerError: return QObject::tr("Casting pointer error: ") + m_additionalErrorInfo;

        case ErrorType::GlobalObjectError: return QObject::tr("JSON global object error: ") + m_additionalErrorInfo;

        case ErrorType::HeaderNotFound: return QObject::tr("Header not found in config file!");
        case ErrorType::HeaderNameNotFound: return QObject::tr("'name' field not found in header");
        case ErrorType::HeaderVersionNotFound: return QObject::tr("'version' field not found in header");
        case ErrorType::HeaderRegisterSizeNotFound: return QObject::tr("'size' field not found in header");
        case ErrorType::HeaderDataOrderNotFound: return QObject::tr("'order' field not found in header");

        case ErrorType::RegisterMapNotFound: return QObject::tr("Register map not found in config file!");
        case ErrorType::RegisterHeaderError: return QObject::tr("Error in register header: ") + m_additionalErrorInfo;
        case ErrorType::RegisterContentError: return QObject::tr("Register parse error:") + m_additionalErrorInfo;

        case ErrorType::FieldContentError: return QObject::tr("Field content error: ") + m_additionalErrorInfo;
        case ErrorType::FieldSizeError: return QObject::tr("Field size error: ") + m_additionalErrorInfo;
        case ErrorType::FieldsOverLaps: return QObject::tr("Fields overlaps: ") + m_additionalErrorInfo;

        default: return QObject::tr("Unknown error");
    }
}

void ParseError::setErrorType(ErrorType type, const QString &additionalErrorInfo)
{
    m_errorType = type;
    m_additionalErrorInfo = additionalErrorInfo;
}
