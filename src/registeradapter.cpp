#include "registeradapter.h"

QString RegisterAdapter::name() const
{
    return m_register->name();
}

quint16 RegisterAdapter::fieldsCount()
{
    return m_register->fieldsCount();
}

FieldAdapter RegisterAdapter::field(quint16 fieldIndex)
{

    return FieldAdapter(&m_register->field(fieldIndex));
}

QString RegisterAdapter::value()
{
    QByteArray registerData = m_register->rawData();

    std::reverse(registerData.begin(), registerData.end());
    QString result = registerData.toHex('-');
    return result;
}
