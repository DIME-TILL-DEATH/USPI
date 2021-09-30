#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <QFile>
#include <QDir>
#include <QString>

#include <QDebug>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "dutdevice.h"
#include "register.h"
#include "parseerror.h"

#include "abstractfield.h"
#include "bitfield.h"
#include "integerfield.h"
#include "variantlistfield.h"
#include "fixedfield.h"
#include "separationfield.h"

class FileParser
{
public:
    FileParser();

    bool loadFile(const QString& name, ParseError* error = nullptr);

    bool readHeader(DUTDevice::Header *header, ParseError* error = nullptr);
    bool readRegisterArray(std::vector<std::shared_ptr<Register> > *registerMap, DUTDevice::Header *header, ParseError* error = nullptr);

private:
    QJsonObject m_deviceGlobalObject;

    bool readRegister(const QJsonObject &jsonObject, Register* deviceRegister, ParseError* error = nullptr);

    static bool readAbstractField(const QJsonObject &jsonObject, AbstractField* field, ParseError* error = nullptr);

    static bool readBitField(const QJsonObject &jsonObject, Register* deviceRegister, ParseError* error = nullptr);
    static bool readIntegerField(const QJsonObject &jsonObject, Register* deviceRegister, ParseError* error = nullptr);
    static bool readVariantListField(const QJsonObject &jsonObject, Register* deviceRegister,  ParseError* error = nullptr);
    static bool readFixedField(const QJsonObject& jsonObject, Register* deviceRegister, ParseError* error = nullptr);
    static bool readSeparationField(const QJsonObject& jsonObject, Register* deviceRegister, ParseError* error = nullptr);

    std::map<QString, std::function<bool(const QJsonObject&, Register*, ParseError*)> > readMethodList
    {
        {"bits",            &FileParser::readBitField},
        {"integers",        &FileParser::readIntegerField},
        {"variant_lists",   &FileParser::readVariantListField},
        {"fixed",           &FileParser::readFixedField},
        {"separators",      &FileParser::readSeparationField}
    };

    static bool parseFieldIntObject(const QJsonObject& jsonObject, const QString& valueName, quint64& destValue,
                        const QString& fieldName = "unknown", quint64 defaultValue = 0, bool mandatory = false,  ParseError* error = nullptr);

    static bool parseFieldDoubleObject(const QJsonObject& jsonObject, const QString& valueName, qreal& destValue,
                        const QString& fieldName = "unknown", qreal defaultValue = 0, bool mandatory = false,  ParseError* error = nullptr);

    static bool parseFieldStringObject(const QJsonObject& jsonObject, const QString& valueName, QString& destValue,
                        const QString& fieldName = "unknown", bool mandatory = false,  ParseError* error = nullptr);
};

#endif // FILEPARSER_H
