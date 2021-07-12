#ifndef ABSTRACTINTERFACE_H
#define ABSTRACTINTERFACE_H

#include <QByteArray>

#include "register.h"

class AbstractInterface
{
public:
    AbstractInterface();


    virtual bool writeRegister(const Register& wrReg);
    virtual bool writeSequence(const std::vector<Register>& wrSequence);


    const QString &interfaceName() const;

protected:
    QString m_interfaceName;
};

#endif // ABSTRACTINTERFACE_H
