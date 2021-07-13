#ifndef REGISTERADAPTER_H
#define REGISTERADAPTER_H

#include <QObject>
#include <QVariant>

#include "fieldadapter.h"
#include "register.h"

class RegisterAdapter
{
    Q_GADGET
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(quint16 fieldsCount READ fieldsCount)
public:
    RegisterAdapter() {};
    RegisterAdapter(Register* reg) : m_register{reg} {};

    QString name() const;
    quint16 fieldsCount();

    Q_INVOKABLE FieldAdapter field(quint16 fieldIndex);

    Q_INVOKABLE QString value();

private:
    Register* m_register;
};
Q_DECLARE_METATYPE(RegisterAdapter)

#endif // REGISTERADAPTER_H
