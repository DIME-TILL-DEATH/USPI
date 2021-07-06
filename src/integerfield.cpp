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
