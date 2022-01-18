#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QDebug>
#include <vector>

//#include "abstractinterface.h"
#include "devicepin.h"

class AbstractInterface;
class AbstractController
{
public:
    AbstractController(AbstractInterface* interface, const QString& name, const QMap<QString, DevicePin> &m_devicePins);

    virtual bool setPinFunction(const DevicePin& pin, DevicePin::Function function);

    const QString &deviceName() const;
    const QMap<QString, DevicePin> &devicePins() const;

private:
    AbstractInterface* m_interface;

    QString m_deviceName;
    QMap<QString, DevicePin> m_devicePins;
};

#endif // ABSTRACTCONTROLLER_H
