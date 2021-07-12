#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QObject>
#include <QUrl>
#include <QStringListModel>

#include "dutdevice.h"

class UserInterface : public QObject
{
    Q_OBJECT
public:
    explicit UserInterface(QObject *parent = nullptr);

    Q_INVOKABLE QString loadDevice(const QUrl& fileName);

    // заглушка
    Q_INVOKABLE Register getRegister(quint16 index = 0);
    Q_INVOKABLE quint16 registerCount();
private:
    DUTDevice m_device;
    QStringListModel logger;

signals:

};

#endif // USERINTERFACE_H
