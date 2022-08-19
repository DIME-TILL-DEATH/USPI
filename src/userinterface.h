#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QObject>
#include <QUrl>
#include <QStringListModel>
#include <QSettings>

#include <memory>

#include "logger.h"
#include "dutdevice.h"
#include "dutlistmodel.h"
#include "registeradapter.h"
#include "registerlistmodel.h"
#include "sessionsaver.h"
#include "extensionmanager.h"

#include "abstractinterface.h"
#include "fileinterface.h"
#include "usbinterface.h"
#include "ethernetinterface.h"

struct UserSettings
{
    Q_GADGET
public:
    quint16 m_windowWidth;
    quint16 m_windowHeight;

    Q_PROPERTY(quint16 windowWidth MEMBER m_windowWidth)
    Q_PROPERTY(quint16 windowHeight MEMBER m_windowHeight)
};
Q_DECLARE_METATYPE(UserSettings)

// Слишком большой класс? надо разделить ответственности?
class UserInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString dutDeviceName READ dutDeviceName NOTIFY dutUpdated)
    Q_PROPERTY(QString currentInterface READ currentInterface WRITE setCurrentInterface NOTIFY interfaceUpdated)
    Q_PROPERTY(QStringList avaliableInterfaces READ avaliableInterfaces NOTIFY avaliableInterfacesUpdated)
    Q_PROPERTY(QStringList avaliablePlugins READ avaliablePlugins NOTIFY avaliablePluginsUpdated)

    Q_PROPERTY(UserSettings userSettings READ userSettings WRITE setUserSettings NOTIFY userSettingsUpdated)
public:
    explicit UserInterface(QHash <QString, AbstractInterface* >* avaliableInterfaces, QObject *parent = nullptr);
    ~UserInterface();

    static void registerTypes();

    const QString& dutDeviceName() const;
    const QString& currentInterface() const;
    bool setCurrentInterface(const QString& interfaceName);

    RegisterListModel *currentRegMapModel();
    RegisterListModel *registerSequenceModel();
    DutListModel *dutListModel();

    QStringList avaliableInterfaces();
    QStringList avaliablePlugins();

    Q_INVOKABLE bool loadDevice(const QUrl& fileName);
    Q_INVOKABLE bool writeSequence();

    Q_INVOKABLE bool loadSession(const QUrl& fileName);
    Q_INVOKABLE bool saveSession(const QUrl& fileName);

    Q_INVOKABLE void runPlugin(QString pluginName);

    Q_INVOKABLE void updateAvaliableInterfaces();

    Q_INVOKABLE void addRegisterToSequence(const RegisterAdapter& regAdapter, qint16 index = -1);
    Q_INVOKABLE void removeRegisterFromSequence(qint16 index);

    Q_INVOKABLE void setCurrentRegisterMap(qint16 index);

    const UserSettings &userSettings() const;
    void setUserSettings(const UserSettings &newSettings);



private:

    std::vector<std::shared_ptr<DUTDevice> > m_dutList;
    DUTDevice m_device;
    DutListModel m_dutListModel{&m_dutList};

    ExtensionManager extensionManager;

    std::vector<std::shared_ptr<Register> > m_regSequenceMap;

    RegisterListModel m_currentRegMapModel;
    RegisterListModel m_regSequenceModel;

    SessionSaver m_saver;

    QHash <QString, AbstractInterface* >* m_avaliableInterfaces;
    AbstractInterface* m_interface_ptr{nullptr};

    quint32 m_globalRegIdCounter;

    // Лист с указателями?
    AbstractInterface* m_abstractInterface;
    FileInterface* m_fileInterface;
    USBInterface* m_usbInterface;
    EthernetInterface* m_ethernetInterface;

    QSettings m_applicationSettings;

    UserSettings m_userSettings;

signals:
    void dutUpdated();
    void interfaceUpdated();
    void avaliableInterfacesUpdated();
    void avaliablePluginsUpdated();
    void userSettingsUpdated();

public slots:
    void writeCustomSequence(QStringList registerNames);
};

#endif // USERINTERFACE_H
