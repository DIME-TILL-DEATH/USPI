#include "register.h"

Register::Register()
{

}

Register::~Register()
{
}

quint16 Register::fieldsCount()
{
    return m_fields.size();
}

bool Register::sortAndValidateFields(ParseError *error)
{
    if(!validateBounds(error)) return false;

    if(!validateSize(error)) return false;

    sort();
    return true;
}

AbstractField& Register::field(quint16 fieldIndex)
{
    return *m_fields.at(fieldIndex);
}

QByteArray Register::rawData()
{
    const quint16 arraySize = m_bitSize/8;

    char resultDataChar[arraySize];
    char* regData = nullptr;

    memset(resultDataChar, 0, sizeof(resultDataChar));

    for(auto it=m_fields.begin(); it!=m_fields.end(); ++it)
    {
        QByteArray result((*it)->rawData(arraySize));

        regData = result.data();

        for(quint16 i=0; i<arraySize; i++)
        {
            resultDataChar[i] |= regData[i];
        }
    }

    return QByteArray(resultDataChar, arraySize);
}

bool Register::validateSize(ParseError *error)
{
    for(auto it=m_fields.begin(); it != m_fields.end(); ++it)
    {
        if(((*it)->position() + (*it)->size()) > m_bitSize)
        {
            if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldSizeError,
                                                     "'"+(*it)->name() + "' position '" + QString::number((*it)->position(), 10) +
                                                     "' + size '" + QString::number((*it)->size(), 10) + "' is greater than size of target register '" +
                                                     QString::number(m_bitSize,10) +"')'");
            return false;
        }

        switch((*it)->type())
        {
            case AbstractField::FieldType::FixedField:
            {
                FixedField* field_ptr = dynamic_cast<FixedField*>((*it));
                if(field_ptr->data() > pow(2, field_ptr->size()))
                {
                    if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldSizeError, "'"+field_ptr->name()+"'" +
                                                             " fixed field size '" + QString::number(field_ptr->size(), 10) +
                                                             "' is smaller than stored  value '" + QString::number(field_ptr->data(), 10)+"'");
                            return false;
                }
                break;
            }

            case AbstractField::FieldType::IntegerField:
            {
                IntegerField* field_ptr = dynamic_cast<IntegerField*>((*it));
                if(field_ptr->data() > pow(2, field_ptr->size()))
                {
                    if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldSizeError, "'"+field_ptr->name()+"'" +
                                                             " integer field size '" + QString::number(field_ptr->size(), 10) +
                                                             "' is smaller than stored  value '" + QString::number(field_ptr->data(), 10)+"'");
                            return false;


                }

                if(field_ptr->valueFrom() > pow(2, field_ptr->size()))
                {
                    if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldSizeError, "'"+field_ptr->name()+"'" +
                                                             " minimum value '" + QString::number(field_ptr->valueFrom(), 10) +
                                                             "' is greater than avaliable field size '" + QString::number(field_ptr->size(), 10)+"'");
                            return false;


                }

                if(field_ptr->valueTo() > pow(2, field_ptr->size()))
                {
                    if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldSizeError, "'"+field_ptr->name()+"'" +
                                                             " maximum value '" + QString::number(field_ptr->valueTo(), 10) +
                                                             "' is greater than avaliable field size '" + QString::number(field_ptr->size(), 10)+"'");
                            return false;


                }
                break;
            }

            case AbstractField::FieldType::VariantListField:
            {
                VariantListField* field_ptr = dynamic_cast<VariantListField*>(*it);
                for(auto it_hash = field_ptr->data().begin(); it_hash != field_ptr->data().end(); ++it_hash)
                {
                    if(it_hash.value() > pow(2, field_ptr->size()))
                    {
                        if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldSizeError, "'"+field_ptr->name()+"'" +
                                                                 " value '" + QString::number(it_hash.value() , 10) +
                                                                 "' is greater than avaliable field size '" + QString::number(field_ptr->size(), 10)+"'");
                                return false;
                    }
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }
    return true;
}

bool Register::validateBounds(ParseError *error)
{
    for(auto it_checker = m_fields.begin(); it_checker != m_fields.end()-1; ++it_checker )
    {
        quint16 startChecker = (*it_checker)->position();
        quint16 stopChecker = (*it_checker)->position() + (*it_checker)->size() - 1;

        for(auto it_verify = it_checker+1; it_verify != m_fields.end(); ++it_verify )
        {
            quint16 startVerify = (*it_verify)->position();
            quint16 stopVerify = (*it_verify)->position() + (*it_verify)->size() - 1;
            if((startChecker > stopVerify) || (stopChecker < startVerify) )
            {
             //ok
            }
            else
            {
                if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldsOverLaps, "'" + (*it_checker)->name() +
                                                                                                "' and '" + (*it_verify)->name()+"'");
                return false;
            }
        }
    }
    return true;
}

void Register::sort()
{
    AbstractField* temp;
    for(quint16 i=0; i < m_fields.size()-1; i++)
    {
        for(quint16 j=0; j< m_fields.size()-i-1; j++)
        {
            if(m_fields.at(j)->position() > m_fields.at(j+1)->position())
            {
                temp = m_fields.at(j);
                m_fields.at(j) = m_fields.at(j+1);
                m_fields.at(j+1) = temp;
            }
        }
    }
}

//RegisterAdapter &Register::adapter()
//{
//    return m_adapter;
//}

QString Register::name() const
{
    return m_name;
}
