#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QObject>
#include <QUrl>
#include <QStringListModel>

#include "logger.h"
#include "registeradapter.h"
#include "dutdevice.h"
#include "abstractinterface.h"
#include "registerlistmodel.h"
#include "fileinterface.h"
#include "sessionsaver.h"

class UserInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString dutDeviceName READ dutDeviceName NOTIFY dutDeviceUpdated)
    Q_PROPERTY(QString currentInterface READ currentInterface WRITE setCurrentInterface NOTIFY interfaceUpdated)
    Q_PROPERTY(QStringList avaliableInterfaces READ avaliableInterfaces NOTIFY avaliableInterfacesUpdated)
public:
    explicit UserInterface(QHash <QString, AbstractInterface* >* avaliableInterfaces, QObject *parent = nullptr);
    ~UserInterface();

    const QString& dutDeviceName() const;
    const QString& currentInterface() const;
    bool setCurrentInterface(const QString& interfaceName);

    RegisterListModel *registerMapModel();
    RegisterListModel *registerSequenceModel();

    QStringList avaliableInterfaces();

    Q_INVOKABLE bool loadDevice(const QUrl& fileName);
    Q_INVOKABLE bool writeSequence();

    Q_INVOKABLE bool loadSession(const QUrl& fileName);
    Q_INVOKABLE bool saveSession(const QUrl& fileName);

    Q_INVOKABLE void updateAvaliableInterfaces();

private:
    DUTDevice m_device;

    RegisterListModel m_registerMapModel{m_device.deviceRegisterMap()};
    RegisterListModel m_registerSequenceModel;

    SessionSaver m_saver;

    QHash <QString, AbstractInterface* >* m_avaliableInterfaces;
    AbstractInterface* m_interface_ptr{nullptr};

signals:
    void dutDeviceUpdated();
    void interfaceUpdated();
    void avaliableInterfacesUpdated();
};

#endif // USERINTERFACE_H
