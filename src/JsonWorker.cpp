#include "JsonWorker.h"

bool JsonWorker::jsonObjectFromFile(const QString &name, QJsonObject& targetObject)
{
    ParseError error;
    QFile deviceFile(name);

    if(!deviceFile.open(QIODevice::ReadOnly))
    {
        error.setErrorType(ParseError::ErrorType::FileError, name);
        qWarning() << error.errorString();
        return false;
    }
    else
    {
        QByteArray fileData = deviceFile.readAll();

        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(fileData, &jsonError));

        if(jsonDoc.isNull())
        {
            QString additionalErrorInfo{QObject::tr("Ошибка файл json: ") +
                                        jsonError.errorString() +
                                        QObject::tr(" в позиции ") +
                                        QString::number(jsonError.offset, 10)};

            error.setErrorType(ParseError::ErrorType::GlobalObjectError, additionalErrorInfo);
            qWarning() << error.errorString();

            return false;
        }
        targetObject = jsonDoc.object();
        deviceFile.close();
        return true;
    }
}

bool JsonWorker::loadControllerRegMap(QJsonObject globalObject, std::shared_ptr<AbstractController> controller, ParseError* error)
{
    if(!readRegisterArray(globalObject, &controller->m_controllerRegisterMap, &controller->m_controllerHeader, error)) return false;
    return true;
}

bool JsonWorker::readRegisterArray(QJsonObject deviceGlobalObject, std::vector<std::shared_ptr<Register> > *registerMap,
                                   DUTHeader *deviceHeader, ParseError *error)
{
    if(deviceGlobalObject.contains("registers") && deviceGlobalObject["registers"].isArray())
    {
        QJsonArray registerArray = deviceGlobalObject["registers"].toArray();

        registerMap->clear();
        registerMap->reserve(registerArray.size());

        for (int registerIndex = 0; registerIndex < registerArray.size(); ++registerIndex)
        {
            QJsonObject registerObject = registerArray[registerIndex].toObject();
            Register* deviceRegister = new Register(deviceHeader);

            deviceRegister->m_bitSize = deviceHeader->registerSize;

            if(!readRegister(registerObject, deviceRegister, error))
            {
                delete deviceRegister;
                return false;
            }

            deviceRegister->m_uniqueId = registerIndex + 0xAA; // просто смещение, чтобы unqiqueId не мог быть 0

            registerMap->push_back(std::shared_ptr<Register>(deviceRegister));
        }
        return true;
    }
    else
    {
        if(error != nullptr) error->setErrorType(ParseError::ErrorType::RegisterMapNotFound);
        return false;
    }
}

QJsonObject JsonWorker::saveDut(DUTDevice &device)
{
    QJsonObject jsonHeader;

    jsonHeader["name"] = device.m_deviceHeader.deviceName;
    jsonHeader["description"] = device.m_deviceHeader.description;
    jsonHeader["version"] = device.m_deviceHeader.version;
    jsonHeader["register_size"] = device.m_deviceHeader.registerSize;
    jsonHeader["isMSB"] = device.m_deviceHeader.isMSB;
    jsonHeader["channel_num"] = device.m_deviceHeader.channelNumber;
    jsonHeader["uniqueId"] = device.m_deviceHeader.uniqueId;

    QJsonArray jsonRegisterArray;

    for(std::shared_ptr<Register> currentRegister : device.deviceRegisterMap())
    {
        QJsonObject jsonCurrentRegister;
        saveRegister(*currentRegister, jsonCurrentRegister);

        jsonRegisterArray.append(jsonCurrentRegister);
    }

    QJsonObject jsonResult;

    jsonResult["header"] = jsonHeader;
    jsonResult["registers"] = jsonRegisterArray;

    return jsonResult;
}

QJsonArray JsonWorker::saveDutList(std::vector<std::shared_ptr<DUTDevice> > *dutList)
{
    QJsonArray devicesArray;
    quint16 uniqueId = 0;

    for(auto currentDevice = dutList->begin(); currentDevice != dutList->end(); ++currentDevice)
    {
        currentDevice->get()->m_deviceHeader.uniqueId = uniqueId;
        QJsonObject jsonCurrentDevice = JsonWorker::saveDut(*currentDevice->get());
        devicesArray.append(jsonCurrentDevice);
        uniqueId++;
    }
    return devicesArray;
}

DUTDevice* JsonWorker::readDut(QJsonObject jsonObjectDevice)
{
    ParseError error;
    DUTDevice* resultDevice = new DUTDevice;;
    if(jsonObjectDevice.contains("header") && jsonObjectDevice["header"].isObject())
    {
        QJsonObject jsonHeader = jsonObjectDevice["header"].toObject();

        if(jsonHeader.contains("name") && jsonHeader["name"].isString())
        {
            resultDevice->m_deviceHeader.deviceName = jsonHeader["name"].toString();
        }
        else
        {
            qWarning() << "Ошибка при загрузке устройства. Параметр 'name' не найден!";
            delete resultDevice;
            return nullptr;
        }

        if(jsonHeader.contains("description") && jsonHeader["description"].isString())
        {
            resultDevice->m_deviceHeader.description = jsonHeader["description"].toString();
        }

        if(jsonHeader.contains("version") && jsonHeader["version"].isString())
        {
            resultDevice->m_deviceHeader.version = jsonHeader["version"].toString();
        }

        if(jsonHeader.contains("register_size") && jsonHeader["register_size"].isDouble())
        {
            resultDevice->m_deviceHeader.registerSize = jsonHeader["register_size"].toDouble();
        }
        else qWarning() << "Параметр 'register_size' для устройства '" << resultDevice->m_deviceHeader.deviceName << "' не найден! Значение по-умолчанию '32'";

        if(jsonHeader.contains("isMSB") && jsonHeader["isMSB"].isBool())
        {
            resultDevice->m_deviceHeader.isMSB = jsonHeader["isMSB"].toBool();
        }
        else qWarning() << "Параметр 'isMSB' для устройства'" << resultDevice->m_deviceHeader.deviceName << "' не найден! Значение по-умолчанию 'true'";

        if(jsonHeader.contains("channel_num") && jsonHeader["channel_num"].isDouble())
        {
            resultDevice->m_deviceHeader.channelNumber = jsonHeader["channel_num"].toDouble();
        }

        if(jsonHeader.contains("uniqueId") && jsonHeader["uniqueId"].isDouble())
        {
            resultDevice->m_deviceHeader.uniqueId = jsonHeader["uniqueId"].toDouble();
        }
    }
    else
    {
        qWarning() << "Ошибка при загрузке устройства. Параметр 'header' не найден!";
        delete resultDevice;
        return nullptr;
    }

    resultDevice->m_deviceHeader.deviceType = DeviceType::DUT;

    if(!readRegisterArray(jsonObjectDevice, &resultDevice->m_deviceRegisterMap, &resultDevice->m_deviceHeader, &error))
    {
        qWarning() << error.errorString();
        delete resultDevice;
        return nullptr;
    }
    return resultDevice;
}

void JsonWorker::readDutList(QJsonObject jsonGlobalObject, std::vector<std::shared_ptr<DUTDevice> > *dutList, QMap<qint16, DUTHeader *> *deviceReferenceList)
{
    dutList->clear();
    if(jsonGlobalObject.contains("devices") && jsonGlobalObject["devices"].isArray())
    {
        QJsonArray jsonDutArray = jsonGlobalObject["devices"].toArray();
        for (int registerIndex = 0; registerIndex < jsonDutArray.size(); ++registerIndex)
        {
            QJsonObject pluginObject = jsonDutArray[registerIndex].toObject();

            DUTDevice* currentDevice = readDut(pluginObject);
            if(currentDevice != nullptr)
            {
                dutList->push_back(std::shared_ptr<DUTDevice>(currentDevice));
                if(deviceReferenceList != nullptr)
                {
                    deviceReferenceList->insert(currentDevice->deviceHeader().uniqueId, &currentDevice->m_deviceHeader);
                }
            }
        }
    }
    else
    {
        qWarning() << "Описания устройств не найдены в файле";
        dutList = nullptr;
    }
}

QJsonArray JsonWorker::saveWriteSequence(RegisterListModel* registerWriteSequenceModel)
{
        QJsonArray jsonWriteSequenceArray;

        int index =0;
        foreach(RegisterAdapter currentAdapter, registerWriteSequenceModel->registerAdaptersList())
        {
            QJsonObject jsonItem;

            JsonWorker::saveRegister(*currentAdapter.getRegister(), jsonItem);
            jsonItem["parent_uniqueID"] = currentAdapter.getRegister()->parentDUTHeader()->uniqueId;

            jsonWriteSequenceArray.append(jsonItem);
            index++;
        }
        return jsonWriteSequenceArray;
}

bool JsonWorker::readWriteSequence(QJsonObject globalObject, const QMap<qint16, DUTHeader *>& deviceReferenceList,
                                   std::vector<std::shared_ptr<Register> >* regSequenceMap, RegisterListModel* registerWriteSequenceModel)
{
    registerWriteSequenceModel->resetModel();
    regSequenceMap->clear();

    if(globalObject.contains("write sequence") && globalObject["write sequence"].isArray())
    {
        QJsonArray sequnceArray = globalObject["write sequence"].toArray();

        for (int registerIndex = 0; registerIndex < sequnceArray.size(); ++registerIndex)
        {
            quint16 parentUniqueId;
            QJsonObject variantObject = sequnceArray[registerIndex].toObject();
            if(variantObject.contains("parent_uniqueID"))
            {
                parentUniqueId= variantObject["parent_uniqueID"].toInt();
            }
            else
            {
                qWarning() << "Не найден параметр 'parent_uniqueID";
                return false;
            }

            Register* reg_ptr = new Register();
            JsonWorker::readRegister(variantObject, reg_ptr);
            reg_ptr->m_parentDUTHeader = deviceReferenceList.value(parentUniqueId, nullptr);

            if(reg_ptr->m_parentDUTHeader == nullptr)
            {
                qWarning() << "Ссылка на заголовок устройства для регистра '" << reg_ptr->name() << "' не найдена!";
                delete reg_ptr;
                return false;
            }

            regSequenceMap->push_back(std::shared_ptr<Register>(reg_ptr));

            RegisterAdapter adapter(regSequenceMap->back());
            registerWriteSequenceModel->addItem(adapter, registerIndex);
        }
        return true;
    }
    else return false;
}

bool JsonWorker::readPluginsArray(QJsonObject globalObject, std::vector<PluginInfo>* pluginsList, QMap<qint16, DUTHeader *>* deviceReferenceList)
{
    if(globalObject.contains("plugins") && globalObject["plugins"].isArray())
    {
        QJsonArray jsonPlugInsData = globalObject["plugins"].toArray();

        pluginsList->clear();
        pluginsList->reserve(jsonPlugInsData.size());

        for (int registerIndex = 0; registerIndex < jsonPlugInsData.size(); ++registerIndex)
        {
            QJsonObject pluginObject = jsonPlugInsData[registerIndex].toObject();

            QString name, path, description;

            if(!parseFieldStringObject(pluginObject, "name", name, "", true))
            {
                qWarning() << QObject::tr("Error parsing json file. Plugin name not found!");
                return false;
            }
            if(!parseFieldStringObject(pluginObject, "path", path, "", true))
            {
                qWarning() << QObject::tr("Error parsing json file. Plugin path not found!");
                return false;
            }
            parseFieldStringObject(pluginObject, "description", description);

            QMap<QString,QString> settingsMap;

            if(pluginObject.contains("settings") && pluginObject["settings"].isObject())
            {
                QJsonObject jsonSettingsObject = pluginObject["settings"].toObject();

                QStringList keyList = jsonSettingsObject.keys();

                foreach(QString currentKey, keyList)
                {
                    settingsMap.insert(currentKey, jsonSettingsObject[currentKey].toString());
                }
            }

            PluginInfo currentPlugin(name, path, description, settingsMap);

            if(deviceReferenceList != nullptr)
            {
                quint16 parentUniqueId;
                if(pluginObject.contains("parent_uniqueID"))
                {
                    parentUniqueId= pluginObject["parent_uniqueID"].toInt();
                }
                else
                {
                    qWarning() << "Не найден параметр 'parent_uniqueID' для расширения " << name;
                    return false;
                }
                currentPlugin.setTargetDevice(deviceReferenceList->value(parentUniqueId)->device_ptr);
            }

            pluginsList->push_back(currentPlugin);
        }
        return true;
    }
    else return false;
}

QJsonArray JsonWorker::savePlugInsArray(const std::vector<PluginInfo>& pluginsList)
{
    QJsonArray jsonPlugInsArray;
    foreach(PluginInfo currentPlugIn, pluginsList)
    {
        QJsonObject jsonCurrentPlugIn;

        jsonCurrentPlugIn["name"] = currentPlugIn.name();
        jsonCurrentPlugIn["description"] = currentPlugIn.description();
        jsonCurrentPlugIn["path"] = currentPlugIn.path();
        jsonCurrentPlugIn["parent_uniqueID"] = currentPlugIn.targetDevice()->deviceHeader().uniqueId;

        QJsonObject jsonPlugInSettingsObject;

        QMap<QString,QString> plugInSettings = currentPlugIn.settings();

        for(auto currentSetting = plugInSettings.begin(); currentSetting != plugInSettings.end(); ++currentSetting)
        {
            jsonPlugInSettingsObject[currentSetting.key()] = currentSetting.value();
        }

        jsonCurrentPlugIn["settings"] = jsonPlugInSettingsObject;

        jsonPlugInsArray.append(jsonCurrentPlugIn);
    }
    return jsonPlugInsArray;
}

bool JsonWorker::readRegister(const QJsonObject& jsonObject, Register *deviceRegister, ParseError *error)
{
    std::map<QString, std::function<bool(const QJsonObject&, Register*, ParseError*)> > readMethodList
    {
        {"bits",            &JsonWorker::readBitField},
        {"integers",        &JsonWorker::readIntegerField},
        {"variant_lists",   &JsonWorker::readVariantListField},
        {"fixed",           &JsonWorker::readFixedField},
        {"separators",      &JsonWorker::readSeparationField}
    };

    if(!parseFieldStringObject(jsonObject, "name", deviceRegister->m_name, "", true))
    {
        if(error != nullptr) error->setErrorType(ParseError::ErrorType::RegisterHeaderError, "'name' field not found");
        return false;
    }

    if(jsonObject.contains("size") && jsonObject["size"].isDouble())
    {
        deviceRegister->m_bitSize = jsonObject["size"].toDouble();
    }

    // clear previous pointers
    for(auto it = deviceRegister->m_fields.begin(); it!= deviceRegister->m_fields.end(); ++it)
            delete *it;
    deviceRegister->m_fields.clear();

    for(auto it = readMethodList.begin(); it != readMethodList.end(); ++it)
    {
        QString fieldType = it->first;
        if(jsonObject.contains(fieldType) && jsonObject[fieldType].isArray())
        {
            QJsonArray fieldArray = jsonObject[fieldType].toArray();

            for (int fieldIndex = 0; fieldIndex < fieldArray.size(); ++fieldIndex)
            {
                QJsonObject jsonObject = fieldArray[fieldIndex].toObject();

                if(!it->second(jsonObject, deviceRegister, error))
                    return false;
            }
        }
    }

    if(!deviceRegister->sortAndValidateFields(error))
    {
        if(error != nullptr)
        {
            ParseError fieldError;
            fieldError.setErrorType(ParseError::ErrorType::RegisterContentError,
                                    " '" +deviceRegister->name() + "', "+
                                    error->errorString());
            *error = fieldError;
        }
        return false;
    }
    return true;
}

void JsonWorker::saveRegister(const Register &deviceRegister, QJsonObject &jsonRegister)
{
    jsonRegister["name"] = deviceRegister.name();
    jsonRegister["size"] = deviceRegister.bitSize();
    jsonRegister["reg type"] = static_cast<quint8>(deviceRegister.parentDUTHeader()->deviceType);

    QJsonArray jsonBitFieldArray;
    QJsonArray jsonFixedFieldArray;
    QJsonArray jsonIntegerFieldArray;
    QJsonArray jsonVariantListFieldArray;
    QJsonArray jsonSeparationFieldArray;

    foreach(AbstractField* currentField, deviceRegister.m_fields)
    {
        QJsonObject jsonFieldObject;

        saveAbstractField(currentField, jsonFieldObject);

        switch (currentField->m_type)
        {
            case AbstractField::FieldType::BitField:
                saveBitField(dynamic_cast<BitField*>(currentField), jsonFieldObject);
                jsonBitFieldArray.append(jsonFieldObject);
            break;
            case AbstractField::FieldType::FixedField:
                saveFixedField(dynamic_cast<FixedField*>(currentField), jsonFieldObject);
                jsonFixedFieldArray.append(jsonFieldObject);
            break;
            case AbstractField::FieldType::IntegerField:
                saveIntegerField(dynamic_cast<IntegerField*>(currentField), jsonFieldObject);
                jsonIntegerFieldArray.append(jsonFieldObject);
            break;
            case AbstractField::FieldType::SeparationField:
                jsonSeparationFieldArray.append(jsonFieldObject);
            break;
            case AbstractField::FieldType::VariantListField:
                saveVariantListField(dynamic_cast<VariantListField*>(currentField), jsonFieldObject);
                jsonVariantListFieldArray.append(jsonFieldObject);
            break;
            case AbstractField::FieldType::UnknownField: qWarning() << currentField->m_name << " неизвестный тип поля. Сохранение невозможно"; break;
        }
    }

    if(jsonBitFieldArray.size() > 0) jsonRegister["bits"] = jsonBitFieldArray;
    if(jsonIntegerFieldArray.size() > 0) jsonRegister["integers"] = jsonIntegerFieldArray;
    if(jsonFixedFieldArray.size() > 0) jsonRegister["fixed"] = jsonFixedFieldArray;
    if(jsonSeparationFieldArray.size() > 0) jsonRegister["separators"] = jsonSeparationFieldArray;
    if(jsonVariantListFieldArray.size() > 0) jsonRegister["variant_lists"] = jsonVariantListFieldArray;
}

bool JsonWorker::readAbstractField(const QJsonObject &jsonObject, AbstractField *field, ParseError *error)
{
    if(field)
    {
        if(!parseFieldStringObject(jsonObject, "name", field->m_name, "???", true, error)) return false;

        parseFieldStringObject(jsonObject, "description", field->m_description);
        parseFieldStringObject(jsonObject, "comment", field->m_comment);

        quint64 size64;
        if(!parseFieldIntObject(jsonObject, "position", size64, field->m_name, 0, true, error)) return false;
        field->m_position = size64;
    }
    return true;
}

void JsonWorker::saveAbstractField(AbstractField *abstractField, QJsonObject &fieldObject)
{
    fieldObject["name"] = abstractField->m_name;
    fieldObject["description"] = abstractField->m_description;
    fieldObject["comment"] = abstractField->m_comment;
    fieldObject["position"] = abstractField->m_position;
}

bool JsonWorker::readBitField(const QJsonObject &jsonObject, Register* deviceRegister, ParseError *error)
{
    deviceRegister->m_fields.push_back(new BitField);

    AbstractField* field = *(deviceRegister->m_fields.end()-1);

    if(!readAbstractField(jsonObject, field, error))
        return false;

    BitField* bitField = dynamic_cast<BitField*>(field);

    if(bitField)
    {
        bitField->m_type = AbstractField::FieldType::BitField;

        if(jsonObject.contains("default_value") && jsonObject["default_value"].isBool())
        {
           bitField->m_bit = jsonObject["default_value"].toBool();
        }
        return true;
    }
    else
    {
        if(error != nullptr) error->setErrorType(ParseError::ErrorType::PointerError, "casting AbstractField to BitField is unsuccessfull!");
        return false;
    }
}

void JsonWorker::saveBitField(BitField *bitField, QJsonObject& fieldObject)
{
    if(bitField)
    {
        fieldObject["default_value"] = bitField->m_bit;
    }
    else
    {
        qWarning() << QObject::tr("Нулевой указатель на битовое поле");
    }
}

bool JsonWorker::readIntegerField(const QJsonObject &jsonObject, Register* deviceRegister, ParseError *error)
{
    deviceRegister->m_fields.push_back(new IntegerField);

    AbstractField* field = *(deviceRegister->m_fields.end()-1);

    if(!readAbstractField(jsonObject, field, error))
        return false;

    IntegerField* integerField = dynamic_cast<IntegerField*>(field);

    if(integerField)
    {
        integerField->m_type = AbstractField::FieldType::IntegerField;

        quint64 size64;
        if(!parseFieldIntObject(jsonObject, "size", size64, field->m_name, 0, true, error)) return false;
        integerField->m_size = size64;

        parseFieldIntObject(jsonObject, "valueFrom", integerField->m_valueFrom, field->name(), 0);
        parseFieldIntObject(jsonObject, "valueTo", integerField->m_valueTo, field->name(), pow(2, integerField->m_size)-1);
        parseFieldIntObject(jsonObject, "default_value", integerField->m_data, field->name(), integerField->m_valueFrom);

        if(jsonObject.contains("scale") && jsonObject["scale"].isObject())
        {
            QJsonObject scaleObject = jsonObject["scale"].toObject();

            parseFieldDoubleObject(scaleObject, "coefficient", integerField->m_scaleCoefficient, integerField->m_name, 1);
            parseFieldDoubleObject(scaleObject, "exponent", integerField->m_scaleExponent, integerField->m_name, 1);
            parseFieldDoubleObject(scaleObject, "offset", integerField->m_scaleOffset2, integerField->m_name, 0);
            parseFieldDoubleObject(scaleObject, "offset1", integerField->m_scaleOffset1, integerField->m_name, 0);
            parseFieldDoubleObject(scaleObject, "offset2", integerField->m_scaleOffset2, integerField->m_name, integerField->m_scaleOffset2);

            parseFieldStringObject(scaleObject, "units", integerField->m_scaleUnits);
        }
        return true;
    }
    else
    {
        if(error != nullptr) error->setErrorType(ParseError::ErrorType::PointerError, "casting AbstractField to IntegerField is unsucceccfull!");
        return false;
    }
}

void JsonWorker::saveIntegerField(IntegerField *integerField, QJsonObject &fieldObject)
{
    if(integerField)
    {
        fieldObject["size"] = integerField->m_size;

        QString value;
        value.setNum(integerField->m_data, 16);
        fieldObject["default_value"] = "0x" + value;

        value.setNum(integerField->valueFrom(), 16);
        fieldObject["valueFrom"] = "0x" + value;

        value.setNum(integerField->valueTo(), 16);
        fieldObject["valueTo"] = "0x" + value;

        QJsonObject scale;

        scale["coefficient"] = integerField->m_scaleCoefficient;
        scale["exponent"] = integerField->m_scaleExponent;
        scale["offset1"] = integerField->m_scaleOffset1;
        scale["offset2"] = integerField->m_scaleOffset2;
        scale["units"] = integerField->m_scaleUnits;

        fieldObject["scale"] = scale;
    }
    else
    {
        qWarning() << QObject::tr("Нулевой указатель на целочисленное поле");
    }
}

bool JsonWorker::readVariantListField(const QJsonObject &jsonObject, Register* deviceRegister, ParseError *error)
{
    deviceRegister->m_fields.push_back(new VariantListField);

    AbstractField* field = *(deviceRegister->m_fields.end()-1);

    if(!readAbstractField(jsonObject, field, error))
        return false;

    VariantListField* variantListField = dynamic_cast<VariantListField*>(field);

    if(variantListField)
    {
        variantListField->m_type = AbstractField::FieldType::VariantListField;

        quint64 size64;
        if(!parseFieldIntObject(jsonObject, "size", size64, field->m_name, 0, true, error)) return false;
        variantListField->m_size = size64;

        if(jsonObject.contains("variants") && jsonObject["variants"].isArray())
        {
            QJsonArray variantsArray = jsonObject["variants"].toArray();

            for (int registerIndex = 0; registerIndex < variantsArray.size(); ++registerIndex)
            {
                QJsonObject variantObject = variantsArray[registerIndex].toObject();

                QString variantName;
                quint32 variantValue;

                if(variantObject.contains("name") && variantObject.contains("value")
                        && variantObject["name"].isString()) // && variantObject["value"].isDouble())
                {
                    variantName = variantObject["name"].toString();
                    if(variantObject["value"].isDouble())
                    {
                        variantValue = variantObject["value"].toDouble();
                    }
                    else if(variantObject["value"].isString())
                    {
                        QString str = variantObject["value"].toString();
                        bool conversionResult;
                        variantValue = str.toLongLong(&conversionResult, 0);
                        if(!conversionResult)
                        {
                            qWarning() << "Field '" << field->m_name << "' error. Cannot convert variant from string to int.";
                            return false;
                        }
                    }
                    else
                    {
                        if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldContentError, "Variant list field error.");
                        return false;
                    }

                    variantListField->m_data.insert(variantValue, variantName);
                    variantListField->m_selected = variantName;
                }
                else
                {
                    if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldContentError, "Variant list field error.");
                    return false;
                }
            }

            if(jsonObject.contains("default_value"))
            {
               quint64 defaultValue = 0;

               parseFieldIntObject(jsonObject, "default_value", defaultValue, variantListField->name(), 0);

               if(variantListField->m_data.contains(defaultValue))
               {

                    QString defaultString = variantListField->m_data.value(defaultValue);
                    variantListField->m_selected = defaultString;
               }
               else
               {
                   error->setErrorType(ParseError::ErrorType::FieldContentError, "Variant list field '" + field->name() +"' error. No such default value in list.");
                   return false;
               }
            }
        }
        else
        {
            if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldContentError, "Variant list field '" + field->name() +"': there is no variants in variant list");
            return false;
        }
        return true;
    }
    else
    {
        if(error != nullptr) error->setErrorType(ParseError::ErrorType::PointerError, "casting AbstractField to VariantListField is unsucceccfull!");
        return false;
    }
}

void JsonWorker::saveVariantListField(VariantListField *variantListField, QJsonObject &fieldObject)
{
    if(variantListField)
    {
        fieldObject["size"] = variantListField->m_size;

        QString value;
        value.setNum(variantListField->m_data.key(variantListField->m_selected), 16);
        fieldObject["default_value"] = "0x" + value;

        QJsonArray variants;

        for(auto it = variantListField->m_data.begin();
            it != variantListField->m_data.end();
            ++it)
        {
            QJsonObject variantObject;

            variantObject["name"] = it.value();

            value.setNum(it.key(), 16);
            variantObject["value"] = "0x" + value;

            variants.append(variantObject);
        }

        fieldObject["variants"] = variants;
    }
    else
    {
        qWarning() << "Нулевой указатель на целочисленное поле";
    }
}

bool JsonWorker::readFixedField(const QJsonObject &jsonObject, Register* deviceRegister, ParseError *error)
{
    deviceRegister->m_fields.push_back(new FixedField);

    AbstractField* field = *(deviceRegister->m_fields.end()-1);

    if(!readAbstractField(jsonObject, field, error))
        return false;

    FixedField* fixedField = dynamic_cast<FixedField*>(field);

    if(field)
    {
        fixedField->m_type = AbstractField::FieldType::FixedField;

        quint64 size64;
        if(!parseFieldIntObject(jsonObject, "size", size64, field->m_name, 0, true, error)) return false;
        fixedField->m_size = size64;

        if(!parseFieldIntObject(jsonObject, "value", fixedField->m_data, fixedField->m_name, 0, true, error)) return false;
        return true;
    }
    else
    {
        if(error != nullptr) error->setErrorType(ParseError::ErrorType::PointerError, QObject::tr("не удалось преобразовать AbstractField в FixedField!"));
        return false;
    }
}

void JsonWorker::saveFixedField(FixedField *fixedField, QJsonObject &fieldObject)
{
    if(fixedField)
    {
        fieldObject["size"] = fixedField->m_size;

        QString value;
        value.setNum(fixedField->m_data, 16);
        fieldObject["value"] = "0x" + value;
    }
    else
    {
        qWarning() << QObject::tr("Нулевой указатель на фиксированное поле");
    }
}

bool JsonWorker::readSeparationField(const QJsonObject &jsonObject, Register *deviceRegister, ParseError *error)
{
    deviceRegister->m_fields.push_back(new SeparationField);

    AbstractField* field = *(deviceRegister->m_fields.end()-1);

    if(!readAbstractField(jsonObject, field, error))
        return false;

    SeparationField* separationField = dynamic_cast<SeparationField*>(field);

    if(separationField)
    {
        separationField->m_type = AbstractField::FieldType::SeparationField;
        return true;
    }
    else
    {
        if(error != nullptr) error->setErrorType(ParseError::ErrorType::PointerError, "casting AbstractField to SeparationField is unsucceccfull!");
        return false;
    }
}

bool JsonWorker::parseFieldIntObject(const QJsonObject &jsonObject, const QString& valueName, quint64& destValue,
                                const QString &fieldName, quint64 defaultValue, bool mandatory, ParseError *error)
{
    if(jsonObject.contains(valueName) && jsonObject[valueName].isDouble())
    {
        destValue = jsonObject[valueName].toDouble();
    }
    else if(jsonObject.contains(valueName) && jsonObject[valueName].isString())
    {
        QString str = jsonObject[valueName].toString();
        bool conversionResult;
        destValue = str.toLongLong(&conversionResult, 0);
        if(!conversionResult)
        {
            qWarning() << QObject::tr("Ошибка поля '") << fieldName << QObject::tr("'. Не удалось преобразовать значение 'valueTo' из типа string в int. Используется значение по умолчанию: ") << defaultValue;
            destValue = defaultValue;
        }
    }
    else
    {
        if(mandatory)
        {
            if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldContentError, valueName + QObject::tr(" of field '") + fieldName + QObject::tr("' not found."));
            return false;
        }
        else
        {
            destValue = defaultValue;
        }
    }
    return true;
}

bool JsonWorker::parseFieldDoubleObject(const QJsonObject &jsonObject, const QString &valueName, qreal &destValue, const QString &fieldName, qreal defaultValue, bool mandatory, ParseError *error)
{
    if(jsonObject.contains(valueName) && jsonObject[valueName].isDouble())
    {
        destValue = jsonObject[valueName].toDouble();
    }
    else
    {
        if(mandatory)
        {
            if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldContentError, valueName + QObject::tr(" of field '") + fieldName + QObject::tr("' not found."));
            return false;
        }
        else
        {
            destValue = defaultValue;
        }
    }
    return true;
}

bool JsonWorker::parseFieldStringObject(const QJsonObject &jsonObject, const QString &valueName, QString &destValue, const QString &fieldName, bool mandatory, ParseError *error)
{
    if(jsonObject.contains(valueName) && jsonObject[valueName].isString())
    {
        destValue = jsonObject[valueName].toString();
    }
    else
    {
        if(mandatory)
        {
            if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldContentError, valueName + QObject::tr(" of field '") + fieldName + QObject::tr("' not found."));
            return false;
        }
        else
        {
            destValue = "";
        }
    }
    return true;
}
