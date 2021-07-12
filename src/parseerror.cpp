#include "parseerror.h"

QString ParseError::errorString()
{
    switch(m_errorType)
    {
        case ErrorType::NoError: return "No error";

        case ErrorType::FileError: return "Can't open file: " + m_additionalErrorInfo;

        case ErrorType::PointerError: return "Casting pointer error: " + m_additionalErrorInfo;

        case ErrorType::GlobalObjectError: return "JSON global object error: " + m_additionalErrorInfo;

        case ErrorType::HeaderNotFound: return "Header not found in config file!";
        case ErrorType::HeaderNameNotFound: return "'name' field not found in header";
        case ErrorType::HeaderVersionNotFound: return "'version' field not found in header";
        case ErrorType::HeaderRegisterSizeNotFound: return "'size' field not found in header";
        case ErrorType::HeaderDataOrderNotFound: return "'order' field not found in header";

        case ErrorType::RegisterMapNotFound: return "Register map not found in config file!";
        case ErrorType::RegisterHeaderError: return "Error in register header: " + m_additionalErrorInfo;
        case ErrorType::RegisterContentError: return "Register parse error:" + m_additionalErrorInfo;

        case ErrorType::FieldContentError: return "Field content error: " + m_additionalErrorInfo;
        case ErrorType::FieldSizeError: return "Field size error: " + m_additionalErrorInfo;
        case ErrorType::FieldsOverLaps: return "Fields overlaps: " + m_additionalErrorInfo;

        default: return "Unknown error";
    }
}

void ParseError::setErrorType(ErrorType type, const QString &additionalErrorInfo)
{
    m_errorType = type;
    m_additionalErrorInfo = additionalErrorInfo;
}
