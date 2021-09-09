#ifndef INTEGERFIELD_H
#define INTEGERFIELD_H

#include <QDataStream>

#include "abstractfield.h"

class FileParser;
class IntegerField : public AbstractField
{
public:
    friend class FileParser;

    IntegerField() {};
    IntegerField(AbstractField field) : AbstractField{field} {};

    QByteArray rawData(quint16 targetRegisterByteSize);

    quint64 data() const;
    void setData(quint64 newData);

    quint64 valueFrom() const;
    quint64 valueTo() const;

    friend QDataStream& operator<<(QDataStream& stream, const IntegerField& field);
    friend QDataStream& operator>>(QDataStream& stream, IntegerField& field);

    qreal scaleCoefficient() const;
    qreal scaleOffset() const;
    const QString &scaleUnits() const;

private:
    quint64 m_data;

    quint64 m_valueFrom;
    quint64 m_valueTo;

    qreal m_scaleCoefficient{1};
    qreal m_scaleOffset{0};
    QString m_scaleUnits;
};

#endif // INTEGERFIELD_H
