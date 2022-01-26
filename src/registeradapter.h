#ifndef REGISTERADAPTER_H
#define REGISTERADAPTER_H

#include <QObject>
#include <QVariant>
#include <memory>

#include "fieldadapter.h"
#include "register.h"

class SessionSaver;

class RegisterAdapter
{
    Q_GADGET
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(quint16 fieldsCount READ fieldsCount)
    Q_PROPERTY(QString registerType READ registerType)
public:
    friend class SessionSaver;

    RegisterAdapter() {};
    RegisterAdapter(std::shared_ptr<Register> reg);

    static void registerTypes();

    QString name() const;
    quint16 fieldsCount();

    Q_INVOKABLE FieldAdapter field(quint16 fieldIndex);

    Q_INVOKABLE QString value();

    QString registerType();

    Register *getRegister() const;

    bool isLocal() const;
    void setIsLocal(bool newIsLocal);

private:
    std::shared_ptr<Register> m_register;

    QMap<quint16, FieldAdapter> m_bufferedFieldAdapters;

    bool m_isLocal{false};
};
Q_DECLARE_METATYPE(RegisterAdapter)

#endif // REGISTERADAPTER_H
