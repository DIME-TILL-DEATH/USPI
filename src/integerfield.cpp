#include <math.h>

#include "integerfield.h"

QByteArray IntegerField::rawData(quint16 targetRegisterByteSize)
{
    quint16 numByte = m_position / 8;

    QByteArray resultData(targetRegisterByteSize, 0);

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

quint64 IntegerField::valueFrom() const
{
    return m_valueFrom;
}

quint64 IntegerField::valueTo() const
{
    return m_valueTo;
}

qreal IntegerField::scaleCoefficient() const
{
    return m_scaleCoefficient;
}

qreal IntegerField::scaleOffset() const
{
    return m_scaleOffset;
}

const QString &IntegerField::scaleUnits() const
{
    return m_scaleUnits;
}

QDataStream& operator<<(QDataStream& stream, const IntegerField& field)
{
    stream << field.m_data;
    stream << field.m_valueFrom;
    stream << field.m_valueTo;

    stream << field.m_scaleCoefficient;
    stream << field.m_scaleOffset;
    stream << field.m_scaleUnits;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, IntegerField& field)
{
    stream >> field.m_data;
    stream >> field.m_valueFrom;
    stream >> field.m_valueTo;

    stream >> field.m_scaleCoefficient;
    stream >> field.m_scaleOffset;
    stream >> field.m_scaleUnits;
    return stream;
}
