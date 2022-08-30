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
#include "registerlistmodel.h"
#include "register.h"
#include "parseerror.h"

#include "abstractcontroller.h"

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

    bool loadControllerRegMap(std::shared_ptr<AbstractController> controller, ParseError* error);

    bool readHeader(DUTHeader *header, ParseError* error = nullptr);
//    void saveHeader(const DUTHeader &header);

    bool readRegisterArray(std::vector<std::shared_ptr<Register> > *registerMap, DUTHeader* deviceHeader, ParseError* error = nullptr);

    static QJsonObject jsonSaveDut(DUTDevice &device);
    static QJsonArray jsonSaveDutList(std::vector<std::shared_ptr<DUTDevice> > *dutList);

    static DUTDevice *jsonLoadDut(QJsonObject jsonObjectDevice);
//    static std::vector<std::shared_ptr<DUTDevice> > *jsonLoadDutList(QJsonObject jsonGlobalObject);
    static void jsonLoadDutList(QJsonObject jsonGlobalObject, std::vector<std::shared_ptr<DUTDevice> > *dutList,
                                QMap<qint16, DUTHeader *>* deviceReferenceList = nullptr);

    static bool loadWriteSequence(QJsonObject globalObject, const QMap<qint16, DUTHeader *>& deviceReferenceList,
                                  std::vector<std::shared_ptr<Register> >* regSequenceMap, RegisterListModel* registerWriteSequenceModel);

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
