#include <math.h>

#include "integerfield.h"

IntegerField::IntegerField()
{

}

QByteArray IntegerField::rawData(quint16 targetRegisterByteSize)
{
    quint16 numByte = m_position / 8;

    QByteArray resultData;

    resultData.replace(numByte, 5, shiftBitData(m_data));
    resultData.resize(targetRegisterByteSize);

    return resultData;
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
