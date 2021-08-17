#ifndef VARIANTLISTFIELD_H
#define VARIANTLISTFIELD_H

#include <QMap>
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

    const QMap<quint64, QString> &data() const;

    friend QDataStream& operator<<(QDataStream& stream, VariantListField& field);
    friend QDataStream& operator>>(QDataStream& stream, VariantListField& field);

private:
    QString m_selected;
    QMap<quint64, QString> m_data;
};

#endif // VARIANTLISTFIELD_H
