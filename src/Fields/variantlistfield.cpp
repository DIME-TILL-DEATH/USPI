#include "variantlistfield.h"

QByteArray VariantListField::rawData(quint16 targetRegisterByteSize)
{
    quint64 data = m_data.key(m_selected);

    quint16 numByte = m_position / 8;

    QByteArray resultData(targetRegisterByteSize, 0);

    resultData.replace(numByte, sizeof(quint64), shiftBitData(data));
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

bool VariantListField::containsVariant(QString &variant)
{
    QList<QString> variants = m_data.values();
    return variants.contains(variant);
}

const QMap<quint64, QString> &VariantListField::data() const
{
    return m_data;
}

QDataStream& operator<<(QDataStream& stream, VariantListField& field)
{
    stream << field.m_data;
    stream << field.m_selected;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, VariantListField& field)
{
    stream >> field.m_data;
    stream >> field.m_selected;
    return stream;
}
