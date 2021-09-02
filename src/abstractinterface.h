#ifndef ABSTRACTINTERFACE_H
#define ABSTRACTINTERFACE_H

#include <QByteArray>

#include "register.h"
#include "dutdevice.h"

class AbstractInterface
{
public:
    AbstractInterface();
    virtual ~AbstractInterface() {};

    virtual bool writeRegister(Register* wrReg);
    virtual bool writeSequence(const std::vector<Register*> &wrSequence);

    virtual const QString &interfaceName() const;

    const DUTDevice::Header &deviceHeader() const;
    void setDeviceHeader(const DUTDevice::Header &newDeviceHeader);

protected:
    DUTDevice::Header m_deviceHeader;
    bool isActive{true};
    void reverseByte(char& byte);

private:
    QString m_interfaceName{"Abstract"};
};

#endif // ABSTRACTINTERFACE_H
