#ifndef REGISTERADAPTER_H
#define REGISTERADAPTER_H

#include <QObject>
#include <QVariant>
#include <memory>

#include "fieldadapter.h"
#include "register.h"

class RegisterAdapter
{
    Q_GADGET
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(quint16 fieldsCount READ fieldsCount)
public:
    RegisterAdapter() {};
//    RegisterAdapter(Register* reg) : m_register{reg} {};
    RegisterAdapter(std::shared_ptr<Register> reg) : m_register{reg} {};

    static void registerTypes();

    QString name() const;
    quint16 fieldsCount();

    Q_INVOKABLE FieldAdapter field(quint16 fieldIndex);

    Q_INVOKABLE QString value();

    Register *getRegister() const;

    bool isLocal() const;
    void setIsLocal(bool newIsLocal);

private:
    std::shared_ptr<Register> m_register;

    bool m_isLocal{false};
};
Q_DECLARE_METATYPE(RegisterAdapter)

#endif // REGISTERADAPTER_H
