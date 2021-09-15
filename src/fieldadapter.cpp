#include "fieldadapter.h"


void FieldAdapter::registerTypes()
{
    qRegisterMetaType<FieldAdapter>("FieldAdapter");
    qRegisterMetaType<FieldScale>("IntegerFieldScaler");
}

QString FieldAdapter::name() const
{
    return m_field->name();
}

QString FieldAdapter::description() const
{
    return m_field->description();
}

QString FieldAdapter::comment() const
{
    return m_field->comment();
}

QString FieldAdapter::type() const
{
    switch(m_field->type())
    {
        case AbstractField::FieldType::FixedField: return "fixed";
        case AbstractField::FieldType::BitField: return "bit";
        case AbstractField::FieldType::IntegerField: return "integer";
        case AbstractField::FieldType::VariantListField: return "variant_list";
        default: return "unknown";
    }
}

quint16 FieldAdapter::position() const
{
    return m_field->position();
}

quint16 FieldAdapter::size() const
{
    return m_field->size();
}

QVariant FieldAdapter::value() const
{
    switch(m_field->type())
    {
        case AbstractField::FieldType::FixedField:
            return dynamic_cast<FixedField*>(m_field)->data();

        case AbstractField::FieldType::BitField:
            return dynamic_cast<BitField*>(m_field)->getBit();

        case AbstractField::FieldType::IntegerField:
            return dynamic_cast<IntegerField*>(m_field)->data();

        case AbstractField::FieldType::VariantListField:
            return dynamic_cast<VariantListField*>(m_field)->selected();
        return  "variant_list";

        default: return "Value undefined";
    }
}

void FieldAdapter::setValue(const QVariant &newValue)
{

    switch(m_field->type())
    {
        case AbstractField::FieldType::BitField:
            dynamic_cast<BitField*>(m_field)->setBit(newValue.toBool());
            break;
        case AbstractField::FieldType::IntegerField:
        {
            QString tempValue =  newValue.toString();
            bool result;
            dynamic_cast<IntegerField*>(m_field)->setData(tempValue.toLong(&result, 0));
            break;
        }
        case AbstractField::FieldType::VariantListField:
            dynamic_cast<VariantListField*>(m_field)->setSelected(newValue.toString());
        default: break;
    }
}

QVariant FieldAdapter::valueFrom() const
{
    if(m_field->type() == AbstractField::FieldType::IntegerField)
    {
        return dynamic_cast<IntegerField*>(m_field)->valueFrom();
    }
    else
    {
        return 0;
    }
}

QVariant FieldAdapter::valueTo() const
{
    if(m_field->type() == AbstractField::FieldType::IntegerField)
    {
        return dynamic_cast<IntegerField*>(m_field)->valueTo();
    }
    else
    {
        return 0;
    }
}

QVariant FieldAdapter::variantList() const
{
    if(m_field->type() == AbstractField::FieldType::VariantListField)
    {
        QStringList result;
        VariantListField* field_ptr = dynamic_cast<VariantListField*>(m_field);

        for(auto it=field_ptr->data().begin(); it != field_ptr->data().end(); it++)
        {
            result.append(it.value()); // + " - 0x" + QString::number(it.key(), 16));

        }
        return result;
    }
    else
    {
        return value();
    }
}

FieldScale FieldAdapter::fieldScale() const
{
    FieldScale answer;

    answer.coefficient=1;
    answer.offset=0;

    if(m_field->type() == AbstractField::FieldType::IntegerField)
    {
        IntegerField* field_ptr = dynamic_cast<IntegerField*>(m_field);

        answer.coefficient = field_ptr->scaleCoefficient();
        answer.offset = field_ptr->scaleOffset();
        answer.units = field_ptr->scaleUnits();
    }
    return answer;
}

