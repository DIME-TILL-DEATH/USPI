#include "variantlistfield.h"

VariantListField::VariantListField()
{

}

QByteArray VariantListField::rawData(quint16 targetRegisterByteSize)
{
    quint64 data = m_data.value(m_selected);

    quint16 numByte = m_position / 8;

    QByteArray resultData(targetRegisterByteSize, 0);

    resultData.replace(numByte, 5, shiftBitData(data));
    resultData.resize(targetRegisterByteSize);

    return resultData;
}

const QString &VariantListField::selected() const
{
    return m_selected;
}

void VariantListField::setSelected(const QString &newSelected)
{
    m_selected = newSelected;
}

const QHash<QString, quint64> &VariantListField::data() const
{
    return m_data;
}
