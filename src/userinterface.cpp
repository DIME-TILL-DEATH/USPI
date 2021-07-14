#include "userinterface.h"

#include "parseerror.h"

UserInterface::UserInterface(Logger* logDevice, QObject *parent) : QObject(parent),
                                                            m_log{logDevice}
{
    //dummy
    if(m_log == nullptr)
    {
        m_log = new Logger();
    }
}

const QString &UserInterface::dutDeviceName() const
{
    return m_device.name();
}

void UserInterface::setRegisterWriteSequence(const QVector<RegisterAdapter> &sequence)
{
    m_registerWriteSequence=sequence;
}

const QVector<RegisterAdapter> &UserInterface::registerWriteSequence() const
{
    return m_registerWriteSequence;
}

QString UserInterface::loadDevice(const QUrl &fileName)
{
    ParseError error;
    m_device.loadFromFile(fileName.toLocalFile(), &error);
    emit dutDeviceUpdated();
    return error.errorString();
}

bool UserInterface::writeSequence()
{
    m_registerWriteSequence.pop_back();
    if(m_interface_ptr != nullptr )
    {
    }
    else
    {
        m_log->message("Null interface pointer");
        return false;
    }
}

RegisterAdapter UserInterface::registerAdapter(quint16 index)
{
    return m_device.registerAdapter(index);
}

quint16 UserInterface::registerCount()
{
    return m_device.registerCount();
}
