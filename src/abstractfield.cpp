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

quint8 AbstractField::position() const
{
    return m_position;
}

quint8 AbstractField::size() const
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

    QByteArray resultData(5, 0);

    char byteData[5];

    // дополнительно используется один байт для
    // возможности смещения поля на 8 бит

    byteData[0] =   maskedData & 0x00000000ff;
    byteData[1] = ( maskedData & 0x000000ff00 ) >> 8;
    byteData[2] = ( maskedData & 0x0000ff0000 ) >> 16;
    byteData[3] = ( maskedData & 0x00ff000000 ) >> 24;
    byteData[4] = ( maskedData & 0xff00000000 ) >> 32;

    resultData.replace(0, 5, byteData, 5);

    return resultData;
}
