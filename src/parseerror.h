#ifndef PARSEERROR_H
#define PARSEERROR_H

#include <QString>
//#include "fileparser.h"

class FileParser;

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
        FieldContentError
    };

    QString errorString();

    friend class FileParser;
private:
    void setErrorType(ErrorType type, const QString& additionalErrorInfo = "");

    ErrorType m_errorType;
    QString m_additionalErrorInfo;
};

#endif // PARSEERROR_H
