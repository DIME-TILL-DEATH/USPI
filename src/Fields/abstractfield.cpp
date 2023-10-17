#include "abstractfield.h"


AbstractField::AbstractField()
{

}

AbstractField::~AbstractField()
{

}

QByteArray AbstractField::rawData(quint16 targetRegisterSize)
{
    return QByteArray(targetRegisterSize, 0);
}

const QString &AbstractField::name() const
{
    return m_name;
}

const QString &AbstractField::description() const
{
    return m_description;
}

const QString &AbstractField::comment() const
{
    return m_comment;
}

AbstractField::FieldType AbstractField::type() const
{
    return m_type;
}

quint16 AbstractField::position() const
{
    return m_position;
}

quint16 AbstractField::size() const
{
    return m_size;
}

QByteArray AbstractField::shiftBitData(quint64 data)
{
//    quint16 numByte = m_position / 8;
    quint8 numBit = m_position % 8;

    quint64 mask = pow(2, m_size)-1;
    quint64 maskedData = data & mask;

    maskedData = maskedData << numBit;

    QByteArray resultData(8, 0);

    char byteData[sizeof(quint64)];

    // дополнительно используется один байт для
    // возможности смещения поля на 8 бит

    byteData[0] =   maskedData & 0x00000000000000ff;
    byteData[1] = ( maskedData & 0x000000000000ff00 ) >> 8;
    byteData[2] = ( maskedData & 0x0000000000ff0000 ) >> 16;
    byteData[3] = ( maskedData & 0x00000000ff000000 ) >> 24;
    byteData[4] = ( maskedData & 0x000000ff00000000 ) >> 32;
    byteData[5] = ( maskedData & 0x0000ff0000000000 ) >> 40;
    byteData[6] = ( maskedData & 0x00ff000000000000 ) >> 48;
    byteData[7] = ( maskedData & 0xff00000000000000 ) >> 56;

    resultData.replace(0, 8, byteData, 8);

    return resultData;
}

QDataStream& operator<<(QDataStream& stream, const AbstractField& field)
{
    stream << field.m_name;
    stream << field.m_type;
    stream << field.m_position;
    stream << field.m_size;
    stream << field.m_comment;
    stream << field.m_description;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, AbstractField& field)
{
    stream >> field.m_name;
    stream >> field.m_type;
    stream >> field.m_position;
    stream >> field.m_size;
    stream >> field.m_comment;
    stream >> field.m_description;
    return stream;
}
