#include "abstractinterface.h"

AbstractInterface::AbstractInterface(QObject *parent)
                  :QObject(parent)
{
   // For debugging purposes
    std::shared_ptr<AbstractController> defaultUSBController(new AbstractController("ATMega16U2"));
    connectedControllers().push_back(defaultUSBController);
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

std::shared_ptr<AbstractController> AbstractInterface::selectedController()
{
    return m_connectedControllers.at(0);
}

std::vector<std::shared_ptr<AbstractController> > &AbstractInterface::connectedControllers()
{
    return m_connectedControllers;
}

bool AbstractInterface::isAvaliable() const
{
    return m_isAvaliable;
}

void AbstractInterface::reverseByte(char &byte)
{
    byte = ((byte & 0x55) <<  1) | ((byte & 0xAA) >>  1);
    byte = ((byte & 0x33) <<  2) | ((byte & 0xCC) >>  2);
    byte = ((byte & 0x0F) <<  4) | ((byte & 0xF0) >>  4);
}
