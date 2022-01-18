#ifndef SESSIONSAVER_H
#define SESSIONSAVER_H

#include <QFile>
#include <QDataStream>
#include <QDebug>

#include "dutdevice.h"
#include "registeradapter.h"
#include "registerlistmodel.h"
#include "extensionmanager.h"

class SessionSaver
{
public:
    SessionSaver(DUTDevice* device,
                 ExtensionManager *extensionManager,
                 std::vector<std::shared_ptr<Register> > *localRegisterMap,
                 RegisterListModel* registerMap,
                 RegisterListModel* registerWriteSequenceModel);

    bool saveSession(const QString& filePath);
    bool loadSession(const QString& filePath);

private:
    QString m_sessionSaverVersion{"v.0.3"};
    qreal m_compareVersion;

    DUTDevice* m_device;
    ExtensionManager *m_extensionManager;
    std::vector<std::shared_ptr<Register> >* m_localRegisterMap;
    RegisterListModel* m_registerMapModel;
    RegisterListModel* m_registerWriteSequenceModel;

    //QJsonObject saveDUT();
    bool loadDUT(QJsonObject globalObject);

    QJsonArray saveWriteSequence();
    bool loadWriteSequence(QJsonObject globalObject);

    QJsonObject saveProjectSettings();
    bool loadProjectSettings(QJsonObject globalObject);

    QJsonArray savePlugInsSettings();
    bool loadPlugInSettings(QJsonObject globalObject);
};

#endif // SESSIONSAVER_H
