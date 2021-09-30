#include "registeradapter.h"

RegisterAdapter::RegisterAdapter(std::shared_ptr<Register> reg)
    : m_register{reg}
{
    for(quint32 i=0; i<reg->fieldsCount();i++)
    {
        m_bufferedFieldAdapters.insert(i, FieldAdapter(&m_register->field(i)));
    }
}

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
    if(m_bufferedFieldAdapters.contains(fieldIndex))
    {
        return m_bufferedFieldAdapters.value(fieldIndex);
    }
    else
    {
        return FieldAdapter(&m_register->field(fieldIndex));
    }
}

QString RegisterAdapter::value()
{
    //QByteArray registerData = m_register->rawData();
    QString result;
    QList<QByteArray> rawDataList = m_register->rawData();

    for(auto itReg = rawDataList.begin(); itReg != rawDataList.end(); ++itReg)
    {
        QByteArray registerData = (*itReg);

        // MSB. Надо как-то добавить чтобы проверял как выводить для данного устроцства. MSB или LSB
        std::reverse(registerData.begin(), registerData.end());

        result += registerData.toHex('-');
        result += " ";
    }

    return result;
}

Register *RegisterAdapter::getRegister() const
{
    return m_register.get();
}

bool RegisterAdapter::isLocal() const
{
    return m_isLocal;
}

void RegisterAdapter::setIsLocal(bool newIsLocal)
{
    m_isLocal = newIsLocal;
}
