#include "fileparser.h"

FileParser::FileParser()
{

}

bool FileParser::loadFile(const QString &name, ParseError* error)
{
    QFile deviceFile(name);
    if(!deviceFile.open(QIODevice::ReadOnly))
    {
        if(error != nullptr) error->setErrorType(ParseError::ErrorType::FileError, name);
        return false;
    }
    else
    {
        QByteArray fileData = deviceFile.readAll();

        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(fileData, &jsonError));

        if(jsonDoc.isNull())
        {
            QString additionalErrorInfo{"Invalid json file: " +
                                        jsonError.errorString() +
                                        " at position " +
                                        QString::number(jsonError.offset, 10)};

            if(error != nullptr) error->setErrorType(ParseError::ErrorType::GlobalObjectError, additionalErrorInfo);
            return false;
        }
        m_deviceGlobalObject = jsonDoc.object();

        deviceFile.close();
        return true;
    }
}

bool FileParser::readHeader(DUTDevice::Header* header, ParseError *error)
{
    if(m_deviceGlobalObject.contains("header") && m_deviceGlobalObject["header"].isObject())
    {
        QJsonObject jsonHeader = m_deviceGlobalObject["header"].toObject();

        if(jsonHeader.contains("name") && jsonHeader["name"].isString())
        {
            header->deviceName = jsonHeader["name"].toString();
        }
        else
        {
            if(error != nullptr) error->setErrorType(ParseError::ErrorType::HeaderNameNotFound);
            return false;
        }

        if(jsonHeader.contains("version") && jsonHeader["version"].isString())
        {
            header->version = jsonHeader["version"].toString();
        }
        else
        {
            if(error != nullptr) error->setErrorType(ParseError::ErrorType::HeaderVersionNotFound);
            return false;
        }

        if(jsonHeader.contains("register_size") && jsonHeader["register_size"].isDouble())
        {
            header->registerSize = jsonHeader["register_size"].toDouble();
        }
        else
        {
            if(error != nullptr) error->setErrorType(ParseError::ErrorType::HeaderRegisterSizeNotFound);
            return false;
        }

        if(jsonHeader.contains("isMSB") && jsonHeader["isMSB"].isBool())
        {
            header->isMSB = jsonHeader["isMSB"].toBool();
        }
        else
        {
            if(error != nullptr) error->setErrorType(ParseError::ErrorType::HeaderDataOrderNotFound);
            return false;
        }

        return true;
    }
    else
    {
        error->setErrorType(ParseError::ErrorType::HeaderNotFound);
        return false;
    }
}

bool FileParser::readRegisterArray(std::vector<std::shared_ptr<Register> > *registerMap, DUTDevice::Header* header, ParseError *error)
{
    if(m_deviceGlobalObject.contains("registers") && m_deviceGlobalObject["registers"].isArray())
    {
        QJsonArray registerArray = m_deviceGlobalObject["registers"].toArray();

        registerMap->clear();
        registerMap->reserve(registerArray.size());

        for (int registerIndex = 0; registerIndex < registerArray.size(); ++registerIndex)
        {
            QJsonObject registerObject = registerArray[registerIndex].toObject();
            Register* deviceRegister = new Register;

            deviceRegister->m_bitSize = header->registerSize;

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

bool FileParser::readRegister(const QJsonObject& jsonObject, Register *deviceRegister, ParseError *error)
{
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

    return true;
}

bool FileParser::readAbstractField(const QJsonObject &jsonObject, AbstractField *field, ParseError *error)
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

bool FileParser::readBitField(const QJsonObject &jsonObject, Register* deviceRegister, ParseError *error)
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
        if(error != nullptr) error->setErrorType(ParseError::ErrorType::PointerError, "casting AbstractField to BitField is unsucceccfull!");
        return false;
    }
}

bool FileParser::readIntegerField(const QJsonObject &jsonObject, Register* deviceRegister, ParseError *error)
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
            if(scaleObject.contains("coefficient") && scaleObject["coefficient"].isDouble())
            {
                integerField->m_scaleCoefficient = scaleObject["coefficient"].toDouble();
            }

            if(scaleObject.contains("offset") && scaleObject["offset"].isDouble())
            {
                integerField->m_scaleOffset1 = scaleObject["offset"].toDouble();
            }

            parseFieldStringObject(jsonObject, "units", integerField->m_scaleUnits);
        }
        return true;
    }
    else
    {
        if(error != nullptr) error->setErrorType(ParseError::ErrorType::PointerError, "casting AbstractField to IntegerField is unsucceccfull!");
        return false;
    }
}

bool FileParser::readVariantListField(const QJsonObject &jsonObject, Register* deviceRegister, ParseError *error)
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

            if(jsonObject.contains("default_value") && jsonObject["default_value"].isDouble())
            {
               quint64 defaultValue = jsonObject["default_value"].toInt();

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

bool FileParser::readFixedField(const QJsonObject &jsonObject, Register* deviceRegister, ParseError *error)
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
        if(error != nullptr) error->setErrorType(ParseError::ErrorType::PointerError, "casting AbstractField to FixedField is unsucceccfull!");
        return false;
    }
}

bool FileParser::parseFieldIntObject(const QJsonObject &jsonObject, const QString& valueName, quint64& destValue,
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
            qWarning() << "Field '" << fieldName << "' error. Cannot convert 'valueTo' from string to int. Using default value: " << defaultValue;
            destValue = defaultValue;
        }
    }
    else
    {
        if(mandatory)
        {
            if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldContentError, valueName + " of field '" + fieldName + "' not found.");
            return false;
        }
        else
        {
            destValue = defaultValue;
        }
    }
    return true;
}

bool FileParser::parseFieldStringObject(const QJsonObject &jsonObject, const QString &valueName, QString &destValue, const QString &fieldName, bool mandatory, ParseError *error)
{
    if(jsonObject.contains(valueName) && jsonObject[valueName].isString())
    {
        destValue = jsonObject[valueName].toString();
    }
    else
    {
        if(mandatory)
        {
            if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldContentError, valueName + " of field '" + fieldName + "' not found.");
            return false;
        }
        else
        {
            destValue = "";
        }
    }
    return true;
}
