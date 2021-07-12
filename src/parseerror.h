#ifndef PARSEERROR_H
#define PARSEERROR_H

#include <QString>

// Может всё-таки setErrorType сделать открытым?
class FileParser;
class Register;
class DUTDevice;

class ParseError
{
public:
    ParseError() : m_errorType{ErrorType::NoError} {};

    enum class ErrorType
    {
        NoError,

        FileError,

        PointerError,

        GlobalObjectError,

        HeaderNotFound,
        HeaderNameNotFound,
        HeaderVersionNotFound,
        HeaderRegisterSizeNotFound,
        HeaderDataOrderNotFound,

        RegisterMapNotFound,
        RegisterHeaderError,
        RegisterContentError,

        FieldContentError,
        FieldSizeError,
        FieldsOverLaps
    };

    QString errorString();

    friend class FileParser;
    friend class DUTDevice;
    friend class Register;
private:
    void setErrorType(ErrorType type, const QString& additionalErrorInfo = "");

    ErrorType m_errorType;
    QString m_additionalErrorInfo;
};

#endif // PARSEERROR_H
