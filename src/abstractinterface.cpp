#include "abstractinterface.h"

AbstractInterface::AbstractInterface()
{

}

bool AbstractInterface::writeRegister(Register *wrReg)
{
    Q_UNUSED(wrReg)
    return true;
}

bool AbstractInterface::writeSequence(const std::vector<Register *> &wrSequence)
{
    Q_UNUSED(wrSequence)
    return true;
}

const QString &AbstractInterface::interfaceName() const
{
    return m_interfaceName;
}

const DUTDevice::Header &AbstractInterface::deviceHeader() const
{
    return m_deviceHeader;
}

void AbstractInterface::setDeviceHeader(const DUTDevice::Header &newDeviceHeader)
{
    m_deviceHeader = newDeviceHeader;
}
