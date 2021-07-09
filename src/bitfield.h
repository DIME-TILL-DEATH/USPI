#ifndef BITFIELD_H
#define BITFIELD_H

#include "abstractfield.h"

class FileParser;
class BitField : public AbstractField
{
public:
    friend class FileParser;

    BitField();

    QByteArray rawData(quint16 targetRegisterSize);

    bool getBit() const;
    void setBit(bool newSettled);

private:
    bool m_bit;
};

#endif // BITFIELD_H
