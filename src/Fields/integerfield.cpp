#include <math.h>

#include "integerfield.h"

QByteArray IntegerField::rawData(quint16 targetRegisterByteSize)
{
    quint16 numByte = m_position / 8;

    QByteArray resultData(targetRegisterByteSize, 0);

    resultData.replace(numByte, sizeof(quint64), shiftBitData(m_data));
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

qreal IntegerField::scaleOffset1() const
{
    return m_scaleOffset1;
}

const QString &IntegerField::scaleUnits() const
{
    return m_scaleUnits;
}

bool IntegerField::isValidValue() const
{
    if(m_data < m_valueFrom || m_data > m_valueTo) return false;
    else return true;
}

bool IntegerField::isValidValue(quint64 value)
{
    if(value < m_valueFrom || value > m_valueTo) return false;
    else return true;
}

qreal IntegerField::scaleExponent() const
{
    return m_scaleExponent;
}

qreal IntegerField::scaleOffset2() const
{
    return m_scaleOffset2;
}

QDataStream& operator<<(QDataStream& stream, const IntegerField& field)
{
    stream << field.m_data;
    stream << field.m_valueFrom;
    stream << field.m_valueTo;

    stream << field.m_scaleCoefficient;
    stream << field.m_scaleExponent;
    stream << field.m_scaleOffset1;
    stream << field.m_scaleOffset2;
    stream << field.m_scaleUnits;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, IntegerField& field)
{
    stream >> field.m_data;
    stream >> field.m_valueFrom;
    stream >> field.m_valueTo;

    stream >> field.m_scaleCoefficient;
    stream >> field.m_scaleExponent;
    stream >> field.m_scaleOffset1;
    stream >> field.m_scaleOffset2;
    stream >> field.m_scaleUnits;
    return stream;
}
