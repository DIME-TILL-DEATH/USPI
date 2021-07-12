#include "abstractinterface.h"

AbstractInterface::AbstractInterface()
{

}

bool AbstractInterface::writeRegister(const Register &wrReg)
{

}

bool AbstractInterface::writeSequence(const std::vector<Register> &wrSequence)
{

}

const QString &AbstractInterface::interfaceName() const
{
    return m_interfaceName;
}
