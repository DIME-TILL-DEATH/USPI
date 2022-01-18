#include "abstractcontroller.h"

AbstractController::AbstractController(AbstractInterface *interface, const QString &name, const QMap<QString, DevicePin> &devicePins)
    : m_interface{interface},
      m_deviceName{name},
      m_devicePins{devicePins}
{

}

bool AbstractController::setPinFunction(const DevicePin &pin, DevicePin::Function function)
{
    if(!pin.isAvaliableFunction(function)) return false;

    return true;
}

const QMap<QString, DevicePin> &AbstractController::devicePins() const
{
    return m_devicePins;
}

const QString &AbstractController::deviceName() const
{
    return m_deviceName;
}
