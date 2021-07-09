#include "fieldadapter.h"


void FieldAdapter::qt_check_for_QGADGET_macro()
{

}

FieldAdapter::FieldAdapter()
{

}

FieldAdapter::FieldAdapter(AbstractField *field)
    : m_field{field}
{
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
            dynamic_cast<IntegerField*>(m_field)->setData(newValue.toLongLong());
            break;
        case AbstractField::FieldType::VariantListField:


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
