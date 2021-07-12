#include <math.h>

#include "fixedfield.h"

FixedField::FixedField()
{

}

QByteArray FixedField::rawData(quint16 targetRegisterByteSize)
{
    quint16 numByte = m_position / 8;

    QByteArray resultData;

    resultData.replace(numByte, 5, shiftBitData(m_data));
    resultData.resize(targetRegisterByteSize);

    return resultData;
}

quint32 FixedField::data() const
{
    return m_data;
}
