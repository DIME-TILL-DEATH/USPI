#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QDebug>
#include <vector>

//#include "abstractinterface.h"

#include "dutdevice.h"
#include "register.h"
#include "devicepin.h"

class AbstractInterface;
class JsonWorker;
class SessionSaver;

class AbstractController
{
public:
    AbstractController(const QString& name, const QMap<QString, DevicePin> &m_devicePins = {});

    friend class JsonWorker;
    friend class SessionSaver;

    virtual bool setPinFunction(const DevicePin& pin, DevicePin::Function function);

    const QString &deviceName() const;
    const QMap<QString, DevicePin> &devicePins() const;

    const std::vector<std::shared_ptr<Register> >& regMap();
    const QString &regMapFileName() const;

    const DUTHeader& controllerHeader() const;

    virtual QStringList avaliableChannels() = 0;

protected:
    //AbstractInterface* m_interface;
    DUTHeader m_controllerHeader;

    std::vector<std::shared_ptr<Register> > m_controllerRegisterMap{};

    QString m_deviceName;
    QString m_regMapFileName{"ATMega16U2.json"}; // должно считываться из прошивки устройства
    QMap<QString, DevicePin> m_devicePins;
};

#endif // ABSTRACTCONTROLLER_H
