#include "integerfield.h"

IntegerField::IntegerField()
{

}

QByteArray IntegerField::rawData(quint16 targetRegisterSize)
{
    QByteArray result;

    if(m_position + m_size <= targetRegisterSize)
    {
        result.setNum(m_data << m_position, 16);
    }
    return result;
}

quint64 IntegerField::data() const
{
    return m_data;
}

void IntegerField::setData(quint64 newData)
{
    m_data = newData;
}

quint32 IntegerField::valueFrom() const
{
    return m_valueFrom;
}

quint32 IntegerField::valueTo() const
{
    return m_valueTo;
}
