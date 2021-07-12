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

RegisterAdapter UserInterface::registerAdapter(quint16 index)
{
    return m_device.registerAdapter(index);
}

quint16 UserInterface::registerCount()
{
    return m_device.registerCount();
}
