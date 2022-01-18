#ifndef VARIANTLISTFIELD_H
#define VARIANTLISTFIELD_H

#include <QMap>
#include <QDataStream>

#include "abstractfield.h"

class JsonWorker;
class VariantListField : public AbstractField
{
public:
    friend class JsonWorker;

    VariantListField() {m_type = AbstractField::FieldType::VariantListField;};
    VariantListField(AbstractField field) : AbstractField{field} {m_type = AbstractField::FieldType::VariantListField;};

    QByteArray rawData(quint16 targetRegisterByteSize);

    const QString &selected() const;
    void setSelected(const QString &newSelected);

    bool containsVariant(QString& variant);

    const QMap<quint64, QString> &data() const;

    friend QDataStream& operator<<(QDataStream& stream, VariantListField& field);
    friend QDataStream& operator>>(QDataStream& stream, VariantListField& field);

private:
    QString m_selected;
    QMap<quint64, QString> m_data;
};

#endif // VARIANTLISTFIELD_H
