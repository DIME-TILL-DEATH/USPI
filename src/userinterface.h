#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QObject>
#include <QUrl>
#include <QStringListModel>

#include "logger.h"
#include "registeradapter.h"
#include "dutdevice.h"
#include "abstractinterface.h"

class UserInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString dutDeviceName READ dutDeviceName NOTIFY dutDeviceUpdated)
    Q_PROPERTY(QVector<RegisterAdapter> registerWriteSequence READ registerWriteSequence WRITE setRegisterWriteSequence)
public:
    explicit UserInterface(Logger* log = nullptr, QObject *parent = nullptr);

    const QString& dutDeviceName() const;

    void setRegisterWriteSequence(const QVector<RegisterAdapter>& sequence);
    const QVector<RegisterAdapter>& registerWriteSequence() const;

    Q_INVOKABLE QString loadDevice(const QUrl& fileName);

    Q_INVOKABLE bool writeSequence();

    // заглушка
    Q_INVOKABLE RegisterAdapter registerAdapter(quint16 index = 0);
    Q_INVOKABLE quint16 registerCount();


private:
    DUTDevice m_device;

    QVector<RegisterAdapter> m_registerWriteSequence;

    AbstractInterface* m_interface_ptr{nullptr};

    Logger* m_log;

signals:
    void dutDeviceUpdated();

};

#endif // USERINTERFACE_H
