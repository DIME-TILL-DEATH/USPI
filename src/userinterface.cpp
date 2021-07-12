#include "userinterface.h"

#include "parseerror.h"

UserInterface::UserInterface(QObject *parent) : QObject(parent)
{

}

QString UserInterface::loadDevice(const QUrl &fileName)
{
    ParseError error;
    m_device.loadFromFile(fileName.toLocalFile(), &error);
    return error.errorString();
}

Register UserInterface::getRegister(quint16 index)
{
    return m_device.registerAt(index);
}

quint16 UserInterface::registerCount()
{
    return m_device.registerCount();
}
