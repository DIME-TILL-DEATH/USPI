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

}

FieldAdapter Register::field(quint16 fieldIndex)
{
    return FieldAdapter(m_fields.at(fieldIndex), fieldIndex);
}

QString Register::name() const
{
    return m_name;
}
