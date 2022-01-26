#ifndef JSONWORKER_H
#define JSONWORKER_H

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

#include "plugininfo.h"

class JsonWorker
{
public:
    JsonWorker(){};
    JsonWorker(const QJsonObject& globalObject);

    bool loadFile(const QString& name, ParseError* error = nullptr);

    // может надо всё переписать на static? чтобы не надо было нигде создавать объект?
    bool readHeader(DUTDevice::Header *header, ParseError* error = nullptr);
    void saveHeader(const DUTDevice::Header& header);

    bool readRegisterArray(std::vector<std::shared_ptr<Register> > *registerMap, DUTDevice::Header *header, RegisterType registerType, ParseError* error = nullptr);
    void saveRegisterArray(const std::vector<std::shared_ptr<Register> >& registerMap);

    static bool readRegister(const QJsonObject &jsonObject, Register* deviceRegister, ParseError* error = nullptr);
    static void saveRegister(const Register& deviceRegister, QJsonObject& jsonRegister);

    static bool readPluginsArray(QJsonObject globalObject, std::vector<PluginInfo> *pluginsList);
    static void savePlugInsArray(const std::vector<PluginInfo>& pluginsList, QJsonArray& jsonPlugInsArray);

    const QJsonObject &deviceGlobalObject() const;
    void setDeviceGlobalObject(const QJsonObject &newDeviceGlobalObject);

private:
    QJsonObject m_deviceGlobalObject;


    static bool readAbstractField(const QJsonObject &jsonObject, AbstractField* field, ParseError* error = nullptr);
    static void saveAbstractField(AbstractField* abstractField, QJsonObject& fieldObject);

    static bool readBitField(const QJsonObject &jsonObject, Register* deviceRegister, ParseError* error = nullptr);
    static void saveBitField(BitField* bitField, QJsonObject& fieldObject);

    static bool readIntegerField(const QJsonObject &jsonObject, Register* deviceRegister, ParseError* error = nullptr);
    static void saveIntegerField(IntegerField* integerField, QJsonObject& fieldObject);

    static bool readVariantListField(const QJsonObject &jsonObject, Register* deviceRegister,  ParseError* error = nullptr);
    static void saveVariantListField(VariantListField* variantListFieldField, QJsonObject& fieldObject);

    static bool readFixedField(const QJsonObject& jsonObject, Register* deviceRegister, ParseError* error = nullptr);
    static void saveFixedField(FixedField* fixedField, QJsonObject& fieldObject);

    static bool readSeparationField(const QJsonObject& jsonObject, Register* deviceRegister, ParseError* error = nullptr);

    static bool parseFieldIntObject(const QJsonObject& jsonObject, const QString& valueName, quint64& destValue,
                        const QString& fieldName = "unknown", quint64 defaultValue = 0, bool mandatory = false,  ParseError* error = nullptr);

    static bool parseFieldDoubleObject(const QJsonObject& jsonObject, const QString& valueName, qreal& destValue,
                        const QString& fieldName = "unknown", qreal defaultValue = 0, bool mandatory = false,  ParseError* error = nullptr);

    static bool parseFieldStringObject(const QJsonObject& jsonObject, const QString& valueName, QString& destValue,
                        const QString& fieldName = "unknown", bool mandatory = false,  ParseError* error = nullptr);
};

#endif // JSONWORKER_H
