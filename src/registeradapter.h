#ifndef REGISTERADAPTER_H
#define REGISTERADAPTER_H

#include <QObject>
#include <QVariant>

#include "register.h"

class RegisterAdapter
{
    Q_GADGET
    Q_PROPERTY(QString name READ name NOTIFY registerUpdated)
    Q_PROPERTY(quint16 fieldsCount READ fieldsCount NOTIFY registerUpdated)

public:
    RegisterAdapter() {};
    RegisterAdapter(Register* reg) : m_register{reg} {};

    QString name() const;
    quint16 fieldsCount();

    Q_INVOKABLE FieldAdapter field(quint16 fieldIndex);

    // или сделать property
    // либо функция, но с выбором в каком формате отображать
    // но property тоже может быть в разных форматах. Через переменную.
    Q_INVOKABLE QString value();

signals:
    void registerUpdated();

private:
    Register* m_register;
};
Q_DECLARE_METATYPE(RegisterAdapter)

#endif // REGISTERADAPTER_H
