#include "userinterface.h"

#include "parseerror.h"

UserInterface::UserInterface(QHash <QString, AbstractInterface* >* avaliableInterfaces, Logger* logDevice, QObject *parent)
    : QObject(parent),
      m_avaliableInterfaces{avaliableInterfaces},
      m_log{logDevice}
{
    updateAvaliableInterfaces();
    //dummy
    if(m_log == nullptr)
    {
        m_log = new Logger();
    }

    if(m_interface_ptr == nullptr)
    {
        m_interface_ptr = m_avaliableInterfaces->value("File");
    }
}

UserInterface::~UserInterface()
{

}

const QString &UserInterface::dutDeviceName() const
{
    return m_device.name();
}

const QString &UserInterface::currentInterface() const
{
    return m_interface_ptr->interfaceName();
}

bool UserInterface::setCurrentInterface(const QString &interfaceName)
{
    QList<AbstractInterface*> avaliableInterfacesList = m_avaliableInterfaces->values();

    for(auto it = avaliableInterfacesList.begin(); it != avaliableInterfacesList.end(); ++it)
    {
        if((*it)->interfaceName() == interfaceName)
        {
            m_interface_ptr = *it;
            emit interfaceUpdated();
            return true;
        }
    }
    m_log->message("Error in setting interface: interface not found");
    return false;
}

QString UserInterface::loadDevice(const QUrl &fileName)
{
    ParseError error;
    m_device.loadFromFile(fileName.toLocalFile(), &error);

    emit dutDeviceUpdated();
    m_registerMapModel.resetModel(m_device.deviceRegisterMap());

    return error.errorString();
}

bool UserInterface::writeSequence()
{
    if(m_interface_ptr != nullptr )
    {
        QString resultMessage("Запись через '" + m_interface_ptr->interfaceName() + "' в регистры ");

        std::vector<Register*> wrSequence;
        for(auto it = m_registerSequenceModel.registerAdaptersList().begin();
            it != m_registerSequenceModel.registerAdaptersList().end();
            ++it)
        {
            wrSequence.push_back((*it).getRegister());
            resultMessage.append("'"+(*it).name()+"', ");
        }
        resultMessage.chop(2);

        m_interface_ptr->setDeviceHeader(m_device.deviceHeader());

        if(!m_interface_ptr->writeSequence(wrSequence))
        {
            m_log->message("Error while writing sequnce to interface '" + m_interface_ptr->interfaceName()+ "'");
            return false;
        }
        else
        {
            m_log->message(resultMessage);
            return true;
        }
    }
    else
    {
        m_log->message("Null interface pointer");
        return false;
    }
}

QStringList UserInterface::avaliableInterfaces()
{
    QStringList resultList;
    for(auto it = m_avaliableInterfaces->begin(); it != m_avaliableInterfaces->end(); ++it)
    {
        resultList.append(it.key());
    }
    return resultList;
}

void UserInterface::updateAvaliableInterfaces()
{
    m_avaliableInterfaces->clear();

    m_avaliableInterfaces->insert(AbstractInterface().interfaceName(), new AbstractInterface());
    m_avaliableInterfaces->insert(FileInterface().interfaceName(), new FileInterface());

    emit avaliableInterfacesUpdated();
}

RegisterListModel* UserInterface::registerMapModel()
{
    return &m_registerMapModel;
}

RegisterListModel *UserInterface::registerSequenceModel()
{
    return &m_registerSequenceModel;
}
