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

void AbstractInterface::reverseByte(char &byte)
{
    byte = ((byte & 0x55) <<  1) | ((byte & 0xAA) >>  1);
    byte = ((byte & 0x33) <<  2) | ((byte & 0xCC) >>  2);
    byte = ((byte & 0x0F) <<  4) | ((byte & 0xF0) >>  4);
}
