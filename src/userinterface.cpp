#include "userinterface.h"

#include "parseerror.h"

UserInterface::UserInterface(QHash <QString, AbstractInterface* >* avaliableInterfaces, QObject *parent)
    : QObject(parent),
      m_saver{&m_device, &m_localRegisterMap, &m_registerMapModel, &m_registerSequenceModel},
      m_avaliableInterfaces{avaliableInterfaces}
{
    m_abstractInterface = new AbstractInterface();
    m_fileInterface = new FileInterface();
    m_usbInterface = new USB::USBInterface();

    updateAvaliableInterfaces();
    if(m_interface_ptr == nullptr)
    {
        m_interface_ptr = m_avaliableInterfaces->value("File");
    }
}

UserInterface::~UserInterface()
{
    delete m_abstractInterface;
    delete m_fileInterface;
    delete m_usbInterface;
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
    qWarning() << ("Error in setting interface: interface not found");
    return false;
}

bool UserInterface::loadDevice(const QUrl &fileName)
{
    ParseError error;
    if(!m_device.loadFromFile(fileName.toLocalFile(), &error))
    {
        qWarning() << error.errorString();
        return false;
    }

    emit dutDeviceUpdated();
    m_registerMapModel.resetModel(m_device.deviceRegisterMap());

    qInfo() << "Карта регистров для устройства '" << m_device.name() << "' загружена";
    return true;
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
            qWarning() << ("Error while writing sequnce to interface '" + m_interface_ptr->interfaceName()+ "'");
            return false;
        }
        else
        {
            qInfo() << resultMessage;
            return true;
        }
    }
    else
    {
        qWarning() << ("Null interface pointer");
        return false;
    }
}

bool UserInterface::loadSession(const QUrl& fileName)
{
    if(m_saver.loadSession(fileName.toLocalFile()))
    {
        emit dutDeviceUpdated();
        return true;
    }
    else return false;
}

bool UserInterface::saveSession(const QUrl& fileName)
{
    return m_saver.saveSession(fileName.toLocalFile());
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
    QString prevSelectedInterface = m_avaliableInterfaces->key(m_interface_ptr);

    m_avaliableInterfaces->clear();

    m_avaliableInterfaces->insert(m_abstractInterface->interfaceName(), m_abstractInterface);
    m_avaliableInterfaces->insert(m_fileInterface->interfaceName(), m_fileInterface);
    m_avaliableInterfaces->insert(m_usbInterface->interfaceName(), m_usbInterface);

    m_interface_ptr = m_avaliableInterfaces->value(prevSelectedInterface);

    emit avaliableInterfacesUpdated();
    emit interfaceUpdated();
}

void UserInterface::changeWriteItemLocal(quint16 index)
{
    if(!m_registerSequenceModel.registerAdaptersList().at(index).isLocal())
    {
        Register regCopy = *m_registerSequenceModel.registerAdaptersList().at(index).getRegister();
        m_localRegisterMap.push_back(regCopy);

        RegisterAdapter adapter(&(m_localRegisterMap.back()));
        adapter.setIsLocal(true);
        m_registerSequenceModel.changeItem(adapter, index);
    }
    else
    {
        Register& regRef = *m_registerSequenceModel.registerAdaptersList().at(index).getRegister();
        quint16 uniqueId = regRef.uniqueId();
//        m_localRegisterMap.removeOne(regRef);
        m_localRegisterMap.erase(std::find(m_localRegisterMap.begin(), m_localRegisterMap.end(), regRef));

        RegisterAdapter adapter(m_device.registerByUniqueId(uniqueId));
        adapter.setIsLocal(false);
        m_registerSequenceModel.changeItem(adapter, index);
    }
}

RegisterListModel* UserInterface::registerMapModel()
{
    return &m_registerMapModel;
}

RegisterListModel *UserInterface::registerSequenceModel()
{
    return &m_registerSequenceModel;
}
