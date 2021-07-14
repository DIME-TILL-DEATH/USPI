#include "abstractinterface.h"

AbstractInterface::AbstractInterface()
{

}

bool AbstractInterface::writeRegister(const Register &wrReg)
{
    Q_UNUSED(wrReg)
    return true;
}

bool AbstractInterface::writeSequence(const std::vector<Register> &wrSequence)
{
    Q_UNUSED(wrSequence)
    return true;
}

const QString &AbstractInterface::interfaceName() const
{
    return m_interfaceName;
}
