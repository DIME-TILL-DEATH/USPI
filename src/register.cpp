#include "register.h"

Register::Register()
{

}

Register::~Register()
{
    for(auto it = m_fields.begin(); it != m_fields.end(); ++it)
    {
        delete *it;
    }
}

Register::Register(const Register &src_reg)
     : m_uniqueId{src_reg.m_uniqueId},
       m_name{src_reg.m_name},
       m_bitSize{src_reg.m_bitSize}
{
    // Создаём свои объекты AbstractField и ссылки на них помещаем в вектор m_fields
    // объекты будут удалены в деструкторе
    for(auto it = src_reg.m_fields.begin(); it != src_reg.m_fields.end(); ++it)
    {
        switch ((*it)->type())
        {
            case AbstractField::FieldType::BitField:
            {
                BitField* field_ptr = new BitField(*(dynamic_cast<BitField*>(*it)));
                m_fields.push_back(field_ptr);
                break;
            }
            case AbstractField::FieldType::FixedField:
            {
                FixedField* field_ptr = new FixedField(*(dynamic_cast<FixedField*>(*it)));
                m_fields.push_back(field_ptr);
                break;
            }
            case AbstractField::FieldType::IntegerField:
            {
                IntegerField* field_ptr = new IntegerField(*(dynamic_cast<IntegerField*>(*it)));
                m_fields.push_back(field_ptr);
                break;
            }
            case AbstractField::FieldType::VariantListField:
            {
                VariantListField* field_ptr = new VariantListField(*(dynamic_cast<VariantListField*>(*it)));
                m_fields.push_back(field_ptr);
                break;
            }
            default:{break;}
        }
    }
}

bool operator==(const Register& lr, const Register& rr)
{
    if(lr.name() != rr.name()) return false;
    if(lr.uniqueId() != rr.uniqueId()) return false;
    return true;
}

quint16 Register::fieldsCount()
{
    return m_fields.size();
}

bool Register::sortAndValidateFields(ParseError *error)
{
    if(!validateBounds(error)) return false;

    if(!validateSize(error)) return false;

//    sort();
    std::sort(m_fields.begin(), m_fields.end(), [](AbstractField* a, AbstractField* b) {return a->position() < b->position();});
    return true;
}

AbstractField& Register::field(quint16 fieldIndex)
{
    return *m_fields.at(fieldIndex);
}

QList<QByteArray> Register::rawData()
{
    const quint16 arraySize = m_bitSize/8;

    char resultDataChar[arraySize];
    char* regData = nullptr;


    memset(resultDataChar, 0, sizeof(resultDataChar));

    QList<quint16> separationPositions;

    for(auto it=m_fields.begin(); it!=m_fields.end(); ++it)
    {
        if((*it)->type() == AbstractField::FieldType::SeparationField)
        {
            separationPositions.push_back((*it)->position());
            continue;
        }

        QByteArray result((*it)->rawData(arraySize));

        regData = result.data();

        for(quint16 i=0; i<arraySize; i++)
        {
            resultDataChar[i] |= regData[i];
        }
    }

    separationPositions.push_back(m_bitSize-1);

    QByteArray sumResult(resultDataChar, arraySize);
    QList<QByteArray> result;

    quint16 chopPosition = 0;
    quint16 prevChopPosition = 0;

    // chopping for complex registers:
    for(auto it=separationPositions.begin(); it!=separationPositions.end(); ++it)
    {
          chopPosition = ((*it)+1)/8;

        // Работает правильно только для кратных 8 битам!
          result << sumResult.mid(prevChopPosition, chopPosition-prevChopPosition);
          prevChopPosition = chopPosition;
    }

    return result;
}

quint16 Register::uniqueId() const
{
    return m_uniqueId;
}

quint16 Register::bitSize() const
{
    return m_bitSize;
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
                for(auto it_map = field_ptr->data().begin(); it_map != field_ptr->data().end(); ++it_map)
                {
                    if(it_map.key() > pow(2, field_ptr->size()))
                    {
                        if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldSizeError, "'"+field_ptr->name()+"'" +
                                                                 " value '" + QString::number(it_map.key() , 10) +
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
            if((*it_verify)->size() != 0) // skip separators
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
    }
    return true;
}

//void Register::sort()
//{
////    AbstractField* temp;
////    for(quint16 i=0; i < m_fields.size()-1; i++)
////    {
////        for(quint16 j=0; j< m_fields.size()-i-1; j++)
////        {
////            if(m_fields.at(j)->position() > m_fields.at(j+1)->position())
////            {
////                temp = m_fields.at(j);
////                m_fields.at(j) = m_fields.at(j+1);
////                m_fields.at(j+1) = temp;
////            }
////        }
////    }

//    // std library:
//    std::sort(m_fields.begin(), m_fields.end(), [](AbstractField* a, AbstractField* b) {return a->position() < b->position();});
//}

QString Register::name() const
{
    return m_name;
}

QDataStream& operator<<(QDataStream& stream, const Register& reg)
{
    stream << reg.m_name;
    stream << reg.m_uniqueId;
    stream << reg.m_bitSize;

    size_t fieldsCount = reg.m_fields.size();
    stream << fieldsCount;

    for(auto it = reg.m_fields.begin(); it != reg.m_fields.end(); ++it)
    {
        stream << *(*it);

        switch ((*it)->type())
        {
            case AbstractField::FieldType::BitField:
            {
                BitField* field_ptr = dynamic_cast<BitField*>(*it);
                stream << *field_ptr;
                break;
            }
            case AbstractField::FieldType::FixedField:
            {
                FixedField* field_ptr = dynamic_cast<FixedField*>(*it);
                stream << *field_ptr;
                break;
            }
            case AbstractField::FieldType::IntegerField:
            {
                IntegerField* field_ptr = dynamic_cast<IntegerField*>(*it);
                stream << *field_ptr;
                break;
            }
            case AbstractField::FieldType::VariantListField:
            {
                VariantListField* field_ptr = dynamic_cast<VariantListField*>(*it);
                stream << *field_ptr;
                break;
            }
            default:
                stream << *it;
        }
    }
    return stream;
}

QDataStream& operator>>(QDataStream& stream, Register& reg)
{
    stream >> reg.m_name;
    stream >> reg.m_uniqueId;
    stream >> reg.m_bitSize;

    size_t fieldsCount;
    stream >> fieldsCount;

    reg.m_fields.clear();

    for(size_t i=0; i<fieldsCount; ++i)
    {
        AbstractField baseField;
        stream >> baseField;

        switch (baseField.type())
        {
            case AbstractField::FieldType::BitField:
            {
                BitField field(baseField);
                stream >> field;
                reg.m_fields.push_back(new BitField(field));
                break;
            }
            case AbstractField::FieldType::FixedField:
            {
                FixedField field(baseField);
                stream >> field;
                reg.m_fields.push_back(new FixedField(field));
                break;
            }
            case AbstractField::FieldType::IntegerField:
            {
                IntegerField field(baseField);
                stream >> field;
                reg.m_fields.push_back(new IntegerField(field));
                break;
            }
            case AbstractField::FieldType::VariantListField:
            {
                VariantListField field(baseField);
                stream >> field;
                reg.m_fields.push_back(new VariantListField(field));
                break;
            }
            default:
            {
            }
        }
    }
    return stream;
}
