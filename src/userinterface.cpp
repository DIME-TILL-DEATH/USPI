#include <QGuiApplication>

#include "userinterface.h"

#include "JsonWorker.h"
#include "parseerror.h"

UserInterface::UserInterface(QHash <QString, AbstractInterface* >* avaliableInterfaces, QObject *parent)
    : QObject(parent),
      extensionManager(&m_device),
      m_saver{&m_device, &extensionManager, &m_regSequenceMap, &m_deviceRegMapModel, &m_regSequenceModel},
      m_avaliableInterfaces{avaliableInterfaces}
{
    m_abstractInterface = new AbstractInterface(this);
    m_fileInterface = new FileInterface(this);
    m_usbInterface = new USBInterface(this);
    m_ethernetInterface = new EthernetInterface(this);

    updateAvaliableInterfaces();
    if(m_interface_ptr == nullptr)
    {
        //m_interface_ptr = m_avaliableInterfaces->value("USB");
        setCurrentInterface("USB");
    }

    QFont font = QGuiApplication::font();

    m_applicationSettings.beginGroup("UserSettings");

    m_userSettings.m_windowWidth = m_applicationSettings.value("windowWidth", 0).toInt();
    m_userSettings.m_windowHeight = m_applicationSettings.value("windowHeight", 0).toInt();

//    font.setFamily("Times new roman");
//    font.setPointSize(9);

    m_applicationSettings.endGroup();

    font.setStyleStrategy(QFont::NoSubpixelAntialias);
    QGuiApplication::setFont(font);

    connect(&extensionManager, SIGNAL(writeRegisterSequenceRequest()), this, SLOT(writeSequence()));
    connect(&extensionManager, SIGNAL(writeRegisterSequenceRequest(QStringList)), this, SLOT(writeCustomSequence(QStringList)));
}

UserInterface::~UserInterface()
{
    m_applicationSettings.beginGroup("UserSettings");

    m_applicationSettings.setValue("windowWidth", m_userSettings.m_windowWidth);
    m_applicationSettings.setValue("windowHeight", m_userSettings.m_windowHeight);
    m_applicationSettings.setValue("interface", m_interface_ptr->interfaceName());

    m_applicationSettings.endGroup();

//    delete m_abstractInterface;
//    delete m_fileInterface;
//    delete m_usbInterface;
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

    // такой поиск чтобы проверить что интерфейс точно найден QHash->value() возвращает default constr. если не найден
    for(auto it = avaliableInterfacesList.begin(); it != avaliableInterfacesList.end(); ++it)
    {
        if((*it)->interfaceName() == interfaceName)
        {
            m_interface_ptr = *it;

            JsonWorker jsonFile;
            ParseError error;

            QDir dir = QDir::current();
            dir.cd("Controllers");

            // заменить ATMega на имя файла

            if(!jsonFile.loadFile(dir.absoluteFilePath(m_interface_ptr->selectedController()->regMapFileName()), &error))
            {
                qWarning() << error.errorString();
                return false;
            }

            std::shared_ptr<AbstractController> controller = m_interface_ptr->connectedControllers().at(0);

            if(!jsonFile.loadControllerRegMap(controller, &error))//m_interface_ptr->selectedController(), &error))
            {
                qWarning() << error.errorString();
                return false;
            }

            m_controllerRegMapModel.resetModel(controller->regMap());

            emit interfaceUpdated();
            return true;
        }
    }
    qWarning() << (tr("Ошибка выбора интерфейса отправки: интерфейс не найден"));
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
    m_regSequenceModel.resetModel();
    m_regSequenceMap.clear();
    m_deviceRegMapModel.resetModel(m_device.deviceRegisterMap());

    // load plugins for device
    JsonWorker jsonFile;
    if(!jsonFile.loadFile(fileName.toLocalFile(), &error))
    {
        qWarning() << error.errorString();
        return false;
    }

    std::vector<PluginInfo> plugList;

    jsonFile.readPluginsArray(jsonFile.deviceGlobalObject() ,&plugList);

    extensionManager.unloadPlugins();

    extensionManager.loadPlugins(plugList);
    emit avaliablePluginsUpdated();

    qInfo() << tr("Карта регистров для устройства '") << m_device.name() << tr("' загружена");
    return true;
}

bool UserInterface::writeSequence()
{
    if(m_interface_ptr != nullptr )
    {
        if(m_regSequenceModel.registerAdaptersList().size()>0)
        {
            QString resultMessage(tr("Запись через '") + m_interface_ptr->interfaceName() + tr("' в регистры: "));

            std::vector<Register*> wrSequence;
            for(auto it = m_regSequenceModel.registerAdaptersList().begin();
                it != m_regSequenceModel.registerAdaptersList().end();
                ++it)
            {
                wrSequence.push_back((*it).getRegister());
                resultMessage.append("'"+(*it).name()+"', ");
            }
            resultMessage.chop(2);

            m_interface_ptr->setDeviceHeader(m_device.deviceHeader());

            if(!m_interface_ptr->writeSequence(wrSequence))
            {
                qWarning() << (tr("Ошибка записи последовательности через интерфейс '") + m_interface_ptr->interfaceName()+ "'");
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
            qInfo() << tr("Очередь записи пуста");
            return true;
        }
    }
    else
    {
        qWarning() << (tr("Нулевой указатель на интерфейс"));
        return false;
    }
}

bool UserInterface::loadSession(const QUrl& fileName)
{
    if(m_saver.loadSession(fileName.toLocalFile()))
    {
        emit dutDeviceUpdated();
        emit avaliablePluginsUpdated();
        return true;
    }
    else return false;
}

bool UserInterface::saveSession(const QUrl& fileName)
{
    return m_saver.saveSession(fileName.toLocalFile());
}

void UserInterface::runPlugin(QString pluginName)
{
    extensionManager.runPlugin(pluginName);
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

QStringList UserInterface::avaliablePlugins()
{
    return extensionManager.avaliablePlugInsNames();
}

void UserInterface::updateAvaliableInterfaces()
{
    QString prevSelectedInterface;
    if(m_interface_ptr == nullptr)
    {
        m_applicationSettings.beginGroup("UserSettings");

        prevSelectedInterface = m_applicationSettings.value("interface", "USB").toString();

        m_applicationSettings.endGroup();
    }
    else
    {
        prevSelectedInterface = m_avaliableInterfaces->key(m_interface_ptr);
    }

    m_avaliableInterfaces->clear();

    m_avaliableInterfaces->insert(m_abstractInterface->interfaceName(), m_abstractInterface);
    m_avaliableInterfaces->insert(m_fileInterface->interfaceName(), m_fileInterface);

    m_usbInterface->refreshUSBDevices();
    if(m_usbInterface->isAvaliable()) m_avaliableInterfaces->insert(m_usbInterface->interfaceName(), m_usbInterface);

    m_avaliableInterfaces->insert(m_ethernetInterface->interfaceName(), m_ethernetInterface);

    if(m_avaliableInterfaces->contains(prevSelectedInterface))
    {
        //m_interface_ptr = m_avaliableInterfaces->value(prevSelectedInterface);
        setCurrentInterface(prevSelectedInterface);
    }
    else
    {
       // m_interface_ptr = m_avaliableInterfaces->value("File");
        setCurrentInterface("File");
    }

    emit avaliableInterfacesUpdated();
    emit interfaceUpdated();
}

//void UserInterface::changeWriteItemLocal(quint16 index)
//{
//    if(!m_regSequenceModel.registerAdaptersList().at(index).isLocal())
//    {
//        Register* regCopy_ptr = new Register(*(m_regSequenceModel.registerAdaptersList().at(index).getRegister()));
//        m_regSequenceMap.push_back(std::shared_ptr<Register>(regCopy_ptr));

//        RegisterAdapter adapter(m_regSequenceMap.back());
//        adapter.setIsLocal(true);
//        m_regSequenceModel.changeItem(adapter, index);
//    }
//    else
//    {
//        Register* regRef = m_regSequenceModel.registerAdaptersList().at(index).getRegister();
//        quint16 uniqueId = regRef->uniqueId();
//        m_regSequenceMap.erase(std::find_if(m_regSequenceMap.begin(), m_regSequenceMap.end(),
//                                              [&](std::shared_ptr<Register> const& ptr) -> bool {return *ptr == *regRef;}));

//        RegisterAdapter adapter(m_device.registerByUniqueId(uniqueId));
//        adapter.setIsLocal(false);
//        m_regSequenceModel.changeItem(adapter, index);
//    }
//}

void UserInterface::addRegisterToSequence(const RegisterAdapter &regAdapter, qint16 index)
{
    if(index<0) index=0;

    if((quint16)index > m_regSequenceMap.size())
    {
        qWarning() << __FUNCTION__ << ", index больше чем число элеметов";
        index = m_regSequenceMap.size()-1;
    }

    Register* newRegister = new Register(*(regAdapter.getRegister()));
    std::shared_ptr<Register> newRegister_ptr(newRegister);

    m_regSequenceMap.insert(m_regSequenceMap.begin()+index, newRegister_ptr);

    RegisterAdapter adapter(newRegister_ptr);

    m_regSequenceModel.addItem(adapter, index);
}

void UserInterface::removeRegisterFromSequence(qint16 index)
{
    m_regSequenceModel.removeItem(index);
    m_regSequenceMap.erase(m_regSequenceMap.begin()+index);
}

const UserSettings &UserInterface::userSettings() const
{
    return m_userSettings;
}

void UserInterface::setUserSettings(const UserSettings &newSettings)
{
    m_userSettings = newSettings;
}

void UserInterface::writeCustomSequence(QStringList registerNames)
{
    std::vector<Register*> wrSequence;
    foreach(QString name, registerNames)
    {
        wrSequence.push_back(m_device.registerByName(name).get());
    }

    m_interface_ptr->setDeviceHeader(m_device.deviceHeader());

    if(!m_interface_ptr->writeSequence(wrSequence))
    {
        qWarning() << (tr("Ошибка записи последовательности через интерфейс '") + m_interface_ptr->interfaceName()+ "'");
    }
}

void UserInterface::registerTypes()
{
    qRegisterMetaType<UserSettings>("UserSettings");
}

RegisterListModel* UserInterface::deviceRegMapModel()
{
    return &m_deviceRegMapModel;
}

RegisterListModel *UserInterface::controllerRegMapModel()
{
    return &m_controllerRegMapModel;
}

RegisterListModel *UserInterface::registerSequenceModel()
{
    return &m_regSequenceModel;
}
