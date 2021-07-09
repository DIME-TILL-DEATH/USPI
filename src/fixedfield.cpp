#include "fixedfield.h"

FixedField::FixedField()
{

}

QByteArray FixedField::rawData(quint16 targetRegisterSize)
{

}

quint32 FixedField::data() const
{
    return m_data;
}
