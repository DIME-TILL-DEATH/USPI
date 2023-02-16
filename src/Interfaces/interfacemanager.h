#ifndef INTERFACEMANAGER_H
#define INTERFACEMANAGER_H

#include <QObject>

class InterfaceManager : public QObject
{
    Q_OBJECT
public:
    explicit InterfaceManager(QObject *parent = nullptr);

signals:

};

#endif // INTERFACEMANAGER_H
