#include "fieldadapter.h"


FieldAdapter::FieldAdapter()
{

}

FieldAdapter::FieldAdapter(AbstractField *field, quint16 fieldIndex)
    : m_field{field},
      m_index{fieldIndex}
{
}

QString FieldAdapter::name()
{
    return m_field->name();
}

QString FieldAdapter::description()
{
    return m_field->description();
}

QString FieldAdapter::comment()
{
    return m_field->comment();
}

QString FieldAdapter::type()
{
    //return m_field->type();
    switch(m_field->type())
    {
        case AbstractField::FieldType::FixedField: return "fixed";
        case AbstractField::FieldType::BitField: return "bit";
        case AbstractField::FieldType::IntegerField: return "integer";
        case AbstractField::FieldType::VariantListField: return "variant_list";
        default: return "unknown";
    }
}

quint16 FieldAdapter::position()
{
    return m_field->position();
}

quint16 FieldAdapter::size()
{
    return m_field->size();
}

quint16 FieldAdapter::index() const
{
    return m_index;
}
