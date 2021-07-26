#ifndef SESSIONSAVER_H
#define SESSIONSAVER_H

#include <QFile>
#include <QDataStream>
#include <QDebug>

#include "dutdevice.h"
#include "registeradapter.h"
#include "registerlistmodel.h"

class SessionSaver
{
public:
    SessionSaver(DUTDevice* device,
                 RegisterListModel* registerMap,
                 RegisterListModel* registerWriteSequenceModel);

    bool saveSession(const QString& filePath);
    bool loadSession(const QString& filePath);

private:
    QString m_sessionSaverVersion{"v.0.1 alfa"};

    DUTDevice* m_device;
    RegisterListModel* m_registerMapModel;
    RegisterListModel* m_registerWriteSequenceModel;
};

#endif // SESSIONSAVER_H