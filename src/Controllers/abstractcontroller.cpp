#include "abstractcontroller.h"

AbstractController::AbstractController(const QString &name, const QMap<QString, DevicePin> &devicePins)
    : m_deviceName{name},
      m_devicePins{devicePins}
{
    m_controllerHeader.deviceName = "Контроллер";
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

const std::vector<std::shared_ptr<Register> > &AbstractController::regMap()
{
    return m_controllerRegisterMap;
}

const QString &AbstractController::regMapFileName() const
{
    return m_regMapFileName;
}

const DUTHeader &AbstractController::controllerHeader() const
{
    return m_controllerHeader;
}

const QString &AbstractController::deviceName() const
{
    return m_deviceName;
}
