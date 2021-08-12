#ifndef FIELDADAPTER_H
#define FIELDADAPTER_H

#include <QObject>
#include <QVariant>

#include "abstractfield.h"
#include "fixedfield.h"
#include "integerfield.h"
#include "bitfield.h"
#include "variantlistfield.h"

struct FieldScale{
    Q_GADGET
public:
    qreal coefficient;
    qreal offset;
    QString units;

    Q_PROPERTY(qreal coefficient MEMBER coefficient)
    Q_PROPERTY(qreal offset MEMBER offset)
    Q_PROPERTY(QString units MEMBER units)
};
Q_DECLARE_METATYPE(FieldScale)

class FieldAdapter
{
    Q_GADGET

    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(QString comment READ comment)

    Q_PROPERTY(QString type READ type)

    Q_PROPERTY(quint8 position READ position)
    Q_PROPERTY(quint8 size READ size)

    Q_PROPERTY(QVariant value READ value WRITE setValue)
    Q_PROPERTY(QVariant valueFrom READ valueFrom)
    Q_PROPERTY(QVariant valueTo READ valueTo)

    Q_PROPERTY(QVariant variantList READ variantList)

    Q_PROPERTY(FieldScale fieldScale READ fieldScale)

public:
    FieldAdapter() {};
    FieldAdapter(AbstractField* field) : m_field{field} {};

    static void registerTypes();

    QString name() const;
    QString description() const;
    QString comment() const;

    QString type() const;

    quint16 position() const;
    quint16 size() const;

    QVariant value() const;
    void setValue(const QVariant& newValue);

    QVariant valueFrom() const;
    QVariant valueTo() const;

    QVariant variantList() const;

    FieldScale fieldScale() const;

private:
    // при такой логике надо везде проверять что m_field != nullptr
    AbstractField* m_field;

};
Q_DECLARE_METATYPE(FieldAdapter)

#endif // FIELDADAPTER_H
