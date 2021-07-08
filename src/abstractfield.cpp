#include "abstractfield.h"


AbstractField::AbstractField()
{

}

AbstractField::~AbstractField()
{

}

QByteArray AbstractField::rawData(quint16 targetRegisterSize)
{
    return {};
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
