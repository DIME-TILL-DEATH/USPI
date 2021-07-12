#ifndef FIELDADAPTER_H
#define FIELDADAPTER_H

#include <QObject>
#include <QVariant>

#include "abstractfield.h"
#include "fixedfield.h"
#include "integerfield.h"
#include "bitfield.h"
#include "variantlistfield.h"

class FieldAdapter
{
    Q_GADGET

    Q_PROPERTY(QString name READ name NOTIFY fieldUpdated)
    Q_PROPERTY(QString description READ description NOTIFY fieldUpdated)
    Q_PROPERTY(QString comment READ comment NOTIFY fieldUpdated)

    Q_PROPERTY(QString type READ type NOTIFY fieldUpdated)

    Q_PROPERTY(quint8 position READ position NOTIFY fieldUpdated)
    Q_PROPERTY(quint8 size READ size NOTIFY fieldUpdated)

    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY fieldUpdated)
    Q_PROPERTY(QVariant valueFrom READ valueFrom NOTIFY fieldUpdated)
    Q_PROPERTY(QVariant valueTo READ valueTo NOTIFY fieldUpdated)

    Q_PROPERTY(QVariant variantList READ variantList NOTIFY fieldUpdated)

public:
    FieldAdapter();
    FieldAdapter(AbstractField* field);

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
signals:
    void fieldUpdated();

private:
    AbstractField* m_field;

};
Q_DECLARE_METATYPE(FieldAdapter)

#endif // FIELDADAPTER_H
