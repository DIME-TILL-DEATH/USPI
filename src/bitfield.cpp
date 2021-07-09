#include "bitfield.h"

BitField::BitField()
{

}

QByteArray BitField::rawData(quint16 targetRegisterSize)
{
    return {};
}

bool BitField::getBit() const
{
    return m_bit;
}

void BitField::setBit(bool newSettled)
{
    m_bit = newSettled;
}
