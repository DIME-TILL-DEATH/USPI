#include "bitfield.h"

QByteArray BitField::rawData(quint16 targetRegisterByteSize)
{
    QByteArray resultData(targetRegisterByteSize, 0);
    quint16 numByte = m_position/8;
    quint8 numBit = m_position % 8;

    char newByte = m_bit;
    newByte = newByte << numBit;

    resultData.replace(numByte, 1, &newByte, 1);

    return resultData;
}

bool BitField::getBit() const
{
    return m_bit;
}

void BitField::setBit(bool newSettled)
{
    m_bit = newSettled;
}

QDataStream& operator<<(QDataStream& stream, const BitField& field)
{
    stream << field.m_bit;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, BitField& field)
{
    stream >> field.m_bit;
    return stream;
}
