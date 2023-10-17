#include <math.h>

#include "fixedfield.h"

QByteArray FixedField::rawData(quint16 targetRegisterByteSize)
{
    quint16 numByte = m_position / 8;

    QByteArray resultData(targetRegisterByteSize, 0);

    resultData.replace(numByte, sizeof(quint64), shiftBitData(m_data));
    resultData.resize(targetRegisterByteSize);

    return resultData;
}

quint32 FixedField::data() const
{
    return m_data;
}

QDataStream& operator<<(QDataStream& stream, const FixedField& field)
{
    stream << field.m_data;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, FixedField& field)
{
    stream >> field.m_data;
    return stream;
}
