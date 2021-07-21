#ifndef BITFIELD_H
#define BITFIELD_H

#include <QDataStream>

#include "abstractfield.h"

class FileParser;
class BitField : public AbstractField
{
public:
    friend class FileParser;

    BitField() {};
    BitField(AbstractField field) : AbstractField{field} {};

    QByteArray rawData(quint16 targetRegisterByteSize);

    bool getBit() const;
    void setBit(bool newSettled);

    friend QDataStream& operator<<(QDataStream& stream, const BitField& field);
    friend QDataStream& operator>>(QDataStream& stream, BitField& field);

private:
    bool m_bit{false};
};

#endif // BITFIELD_H
