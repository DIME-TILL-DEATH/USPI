#ifndef VARIANTLISTFIELD_H
#define VARIANTLISTFIELD_H

#include <QHash>
#include <QDataStream>

#include "abstractfield.h"

class FileParser;
class VariantListField : public AbstractField
{
public:
    friend class FileParser;

    VariantListField() {};
    VariantListField(AbstractField field) : AbstractField{field} {};

    QByteArray rawData(quint16 targetRegisterByteSize);


    const QString &selected() const;
    void setSelected(const QString &newSelected);

    const QHash<QString, quint64> &data() const;

    friend QDataStream& operator<<(QDataStream& stream, VariantListField& field);
    friend QDataStream& operator>>(QDataStream& stream, VariantListField& field);

private:
    QString m_selected;
    QHash<QString, quint64> m_data;
};

#endif // VARIANTLISTFIELD_H
