#include "registeradapter.h"

void RegisterAdapter::registerTypes()
{
    qRegisterMetaType<RegisterAdapter>("RegisterAdapter");
}

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

    // MSB. Надо как-то добавить чтобы проверял как выводить для данного устроцства. MSB или LSB
    std::reverse(registerData.begin(), registerData.end());
    QString result = registerData.toHex('-');
    return result;
}

Register *RegisterAdapter::getRegister() const
{
    return m_register;
}

bool RegisterAdapter::isLocal() const
{
    return m_isLocal;
}

void RegisterAdapter::setIsLocal(bool newIsLocal)
{
    m_isLocal = newIsLocal;
}
