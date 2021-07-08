#ifndef FIELDADAPTER_H
#define FIELDADAPTER_H

#include <QObject>

#include "abstractfield.h"
#include "integerfield.h"
#include "bitfield.h"
#include "variantlistfield.h"

class FieldAdapter
{
    Q_GADGET

    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(QString comment READ comment)

    Q_PROPERTY(QString type READ type)

    Q_PROPERTY(quint8 position READ position)
    Q_PROPERTY(quint8 size READ size)

    Q_PROPERTY(quint16 index READ index)
public:
    FieldAdapter();
    FieldAdapter(AbstractField* field, quint16 fieldIndex = 0);

    QString name();
    QString description();
    QString comment();

    QString type();

    quint16 position();
    quint16 size();

    quint16 index() const;

private:
    AbstractField* m_field;
    quint16 m_index;
};
Q_DECLARE_METATYPE(FieldAdapter)

#endif // FIELDADAPTER_H
