#include <QGuiApplication>

#include "userinterface.h"

#include "JsonWorker.h"
#include "parseerror.h"

UserInterface::UserInterface(QHash <QString, AbstractInterface* >* avaliableInterfaces, QObject *parent)
    : QObject(parent),
      extensionManager(),
      m_saver{&m_dutList, &extensionManager, &m_regSequenceMap, &m_currentRegMapModel, &m_regSequenceModel}, // Не только текующую, но и все устройства!
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
    m_saver.setInterface(m_interface_ptr);

    QFont font = QGuiApplication::font();

    m_applicationSettings.beginGroup("UserSettings");

    m_userSettings.m_windowWidth = m_applicationSettings.value("windowWidth", 0).toInt();
    m_userSettings.m_windowHeight = m_applicationSettings.value("windowHeight", 0).toInt();

    m_applicationSettings.endGroup();

    font.setStyleStrategy(QFont::NoSubpixelAntialias);
    QGuiApplication::setFont(font);


    connect(&extensionManager, &ExtensionManager::writeSequenceRequest, this, &UserInterface::writeSequence);
    connect(&extensionManager, &ExtensionManager::writeCustomSequenceRequest, this, &UserInterface::writeCustomSequence);

    connect(this, &UserInterface::dutUpdated, &m_dutListModel, &DutListModel::dutUpdated);
    connect(this, &UserInterface::dutUpdated, &m_regSequenceModel, &RegisterListModel::dutUpdated);
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

QString UserInterface::currentInterface() const
{
    QString interfaceName = m_interface_ptr->interfaceName();

//    if(interfaceName == InterfaceNames::USB)
//    {
//        if(m_interface_ptr->isAvaliable())
//        {
//            interfaceName += "(недоступен)";
//        }
//    }

    return interfaceName;
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

            QJsonObject jsonObject;
            ParseError error;

            QDir dir = QDir::current();
            dir.cd("Controllers");

            if(!JsonWorker::jsonObjectFromFile(dir.absoluteFilePath(m_interface_ptr->selectedController()->regMapFileName()), jsonObject))
            {
                return false;
            }

            std::shared_ptr<AbstractController> controller = m_interface_ptr->connectedControllers().at(0);

            if(!JsonWorker::loadControllerRegMap(jsonObject, controller, &error))
            {
                qWarning() << error.errorString();
                return false;
            }

            emit interfaceUpdated();
            return true;
        }
    }
    qWarning() << (tr("Ошибка выбора интерфейса отправки: интерфейс не найден"));
    return false;
}

bool UserInterface::loadDevice(const QUrl &fileName)
{
    QJsonObject jsonObject;
    if(!JsonWorker::jsonObjectFromFile(fileName.toLocalFile(), jsonObject)) return false;

    DUTDevice* newDevice = JsonWorker::readDut(jsonObject);

    if(newDevice == nullptr) return false;

    std::vector<PluginInfo> plugList;

    JsonWorker::readPluginsArray(jsonObject, &plugList);

    for(PluginInfo& it : plugList)
    {
        it.setTargetDevice(newDevice);
    }

    extensionManager.loadPlugins(plugList);

    m_dutListModel.addDutToList(newDevice);

    emit dutUpdated();
    emit avaliablePluginsUpdated();

    qInfo() << tr("Карта регистров для устройства '") << newDevice->name() << tr("' загружена");
    return true;
}

void UserInterface::removeDevice(quint16 index)
{
    m_dutListModel.removeDutFromList(index);
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

//            m_interface_ptr->setDeviceHeader(m_device.deviceHeader());

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
        m_dutListModel.refreshModel();
        emit avaliablePluginsUpdated();
        return true;
    }
    else return false;
}

bool UserInterface::saveSession(const QUrl& fileName)
{
    return m_saver.saveSession(fileName.toLocalFile());
}

void UserInterface::runPlugin(quint16 pluginNumber)
{
    extensionManager.runPlugin(pluginNumber);
}

QStringList UserInterface::avaliableInterfaces()
{
    QStringList resultList;
    for(auto it = m_avaliableInterfaces->begin(); it != m_avaliableInterfaces->end(); ++it)
    {
        QString interfaceName = it.key();

//        if(interfaceName == InterfaceNames::USB)
//        {
//            if(!(*it)->isAvaliable())
//            {
//                interfaceName += "(недоступен)";
//            }
//        }
        resultList.append(interfaceName);
    }
    return resultList;
}

QStringList UserInterface::avaliablePlugins()
{
    return extensionManager.avaliablePlugIns();
}

QStringList UserInterface::avaliableChannels()
{
    return m_interface_ptr->selectedController()->avaliableChannels();
//    return QStringList{{"Кан.№0 - SPI"}, {"Кан.№1 - SPI"}, {"Кан.№2 - SPI"}, {"Кан.№3 - SPI"}};
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
//    if(m_usbInterface->isAvaliable())
//    {
        m_avaliableInterfaces->insert(m_usbInterface->interfaceName(), m_usbInterface);
//    }

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

void UserInterface::setChannelForDevice(qint16 deviceNum, qint16 channelNum)
{
    m_dutList.at(deviceNum)->setChannelNumber(channelNum);
    emit dutUpdated();
    emit avaliablePluginsUpdated();
}

void UserInterface::setCurrentRegisterMap(qint16 index)
{
    if(index == -1)
    {
        m_currentRegMapModel.resetModel(m_interface_ptr->connectedControllers().at(0)->regMap());
    }
    else if(index>=0 && (quint16)index<m_dutList.size())
    {
        m_currentRegMapModel.resetModel(m_dutList.at(index)->deviceRegisterMap());
    }
    else
    {
        qWarning() << __FUNCTION__ << "Incorrect index: " << index;
    }
}

const UserSettings &UserInterface::userSettings() const
{
    return m_userSettings;
}

void UserInterface::setUserSettings(const UserSettings &newSettings)
{
    m_userSettings = newSettings;
}


// Добавить device для которого пишется кастомная последовательность
void UserInterface::writeCustomSequence(QStringList registerNames, DUTDevice *targetDevice)
{
    if(targetDevice == nullptr)
    {
        qWarning() << "Не указано устройство для записи";
        return;
    }

    std::vector<Register*> wrSequence;
    foreach(QString name, registerNames)
    {
        wrSequence.push_back(targetDevice->registerByName(name).get());
    }

//    m_interface_ptr->setDeviceHeader(m_device.deviceHeader());

    if(!m_interface_ptr->writeSequence(wrSequence))
    {
        qWarning() << (tr("Ошибка записи последовательности через интерфейс '") + m_interface_ptr->interfaceName()+ "'");
    }
}

void UserInterface::registerTypes()
{
    qRegisterMetaType<UserSettings>("UserSettings");
}

RegisterListModel *UserInterface::currentRegMapModel()
{
    return &m_currentRegMapModel;
}

RegisterListModel *UserInterface::registerSequenceModel()
{
    return &m_regSequenceModel;
}

DutListModel *UserInterface::dutListModel()
{
    return &m_dutListModel;
}
