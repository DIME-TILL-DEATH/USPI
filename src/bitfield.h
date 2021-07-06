#ifndef BITFIELD_H
#define BITFIELD_H

#include "abstractfield.h"

class FileParser;
class BitField : public AbstractField
{
public:
    BitField();

    QByteArray rawData(quint16 targetRegisterSize);

    friend class FileParser;
};

#endif // BITFIELD_H
