#include "fileparser.h"

FileParser::FileParser()
{

}

bool FileParser::loadFile(const QString &name, ParseError* error)
{
    QFile deviceFile(name);
    if(!deviceFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Config file don't found!";
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
                                        jsonError.offset};

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

bool FileParser::readRegisterArray(std::vector<Register> *registerMap, ParseError *error)
{
    if(m_deviceGlobalObject.contains("registers") && m_deviceGlobalObject["registers"].isArray())
    {
        QJsonArray registerArray = m_deviceGlobalObject["registers"].toArray();

        registerMap->clear();
        registerMap->reserve(registerArray.size());

        for (int registerIndex = 0; registerIndex < registerArray.size(); ++registerIndex)
        {
            QJsonObject registerObject = registerArray[registerIndex].toObject();
            Register deviceRegister;

            if(!readRegister(registerObject, &deviceRegister, error))
                return false;

            registerMap->push_back(deviceRegister);
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
    if(jsonObject.contains("name") && jsonObject["name"].isString())
    {
        deviceRegister->m_name = jsonObject["name"].toString();
    }
    else
    {
        if(error != nullptr) error->setErrorType(ParseError::ErrorType::RegisterHeaderError, "'name' field not found");
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

//                deviceRegister->m_fields.push_back(new AbstractField);

//                if(!it->second(jsonObject, *(deviceRegister->m_fields.end()-1), error))
//                    return false;


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
        if(jsonObject.contains("name") && jsonObject["name"].isString())
        {
            field->m_name = jsonObject["name"].toString();
        }
        else
        {
            if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldContentError, "'name' of field not found.");
            return false;
        }

        if(jsonObject.contains("description") && jsonObject["description"].isString())
        {
            field->m_description = jsonObject["description"].toString();
        }

        if(jsonObject.contains("comment") && jsonObject["comment"].isString())
        {
            field->m_comment = jsonObject["comment"].toString();
        }

        if(jsonObject.contains("position") && jsonObject["position"].isDouble())
        {
            field->m_position = jsonObject["position"].toDouble();
        }
        else
        {
            if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldContentError, "'position' of field not found.");
            return false;
        }
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
//    BitField* bitField = static_cast<BitField*>(field);

    if(bitField)
    {
        bitField->m_type = AbstractField::FieldType::BitField;

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
//        IntegerField* integerField = static_cast<IntegerField*>(field);

    if(integerField)
    {
        integerField->m_type = AbstractField::FieldType::IntegerField;

        if(jsonObject.contains("size") && jsonObject["size"].isDouble())
        {
            integerField->m_size = jsonObject["size"].toDouble();
        }

        if(jsonObject.contains("valueFrom") && jsonObject["valueFrom"].isDouble())
        {
            integerField->m_valueFrom = jsonObject["valueFrom"].toDouble();
            integerField->m_data = integerField->m_valueFrom;
        }

        if(jsonObject.contains("valueTo") && jsonObject["valueTo"].isDouble())
        {
            integerField->m_valueTo = jsonObject["valueTo"].toDouble();
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
//    VariantListField* variantListField = static_cast<VariantListField*>(field);

    if(variantListField)
    {
        variantListField->m_type = AbstractField::FieldType::VariantListField;
        if(jsonObject.contains("size") && jsonObject["size"].isDouble())
        {
            variantListField->m_size = jsonObject["size"].toDouble();
        }

        if(jsonObject.contains("variants") && jsonObject["variants"].isArray())
        {
            QJsonArray variantsArray = jsonObject["variants"].toArray();

            for (int registerIndex = 0; registerIndex < variantsArray.size(); ++registerIndex)
            {
                QJsonObject variantObject = variantsArray[registerIndex].toObject();

                QString variantName;
                quint32 variantValue;

                if(variantObject.contains("name") && variantObject.contains("value")
                        && variantObject["name"].isString() && variantObject["value"].isDouble())
                {
                    variantName = variantObject["name"].toString();
                    variantValue = variantObject["value"].toDouble();

                    variantListField->m_data.insert(variantName, variantValue);
                }
                else
                {
                    if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldContentError, "variant field error.");
                    return false;
                }
            }
        }
        else
        {
            if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldContentError, "there is no variants in variant list");
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

        if(jsonObject.contains("value") && jsonObject["value"].isDouble())
        {
            fixedField->m_data = jsonObject["value"].toDouble();
        }
        else
        {
            if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldContentError, "'value' of fixed field not found");
        }
        return true;
    }
    else
    {
        if(error != nullptr) error->setErrorType(ParseError::ErrorType::PointerError, "casting AbstractField to FixedField is unsucceccfull!");
        return false;
    }
}
