#ifndef SESSIONSAVER_H
#define SESSIONSAVER_H

#include <QFile>
#include <QDataStream>
#include <QDebug>

#include "dutdevice.h"
#include "registeradapter.h"
#include "registerlistmodel.h"
#include "extensionmanager.h"
#include "abstractcontroller.h"
#include "abstractinterface.h"

class SessionSaver
{
public:
    SessionSaver(std::vector<std::shared_ptr<DUTDevice> >* dutList,
                 ExtensionManager *extensionManager,
                 std::vector<std::shared_ptr<Register> > *localRegisterMap,
                 RegisterListModel* registerMap,
                 RegisterListModel* registerWriteSequenceModel);

    bool saveSession(const QString& filePath);
    bool loadSession(const QString& filePath);

    void setInterface(AbstractInterface *newCurrentInterface);

private:
    std::vector<std::shared_ptr<DUTDevice> >* m_deviceList;
    QString m_sessionSaverVersion{"v.0.4"};
    qreal m_compareVersion;

    AbstractInterface* m_currentInterface;
    ExtensionManager* m_extensionManager;
    std::vector<std::shared_ptr<Register> >* m_regSequenceMap;
    RegisterListModel* m_registerMapModel;
    RegisterListModel* m_registerWriteSequenceModel;

    QJsonObject saveProjectSettings();
    bool loadProjectSettings(QJsonObject globalObject);
};

#endif // SESSIONSAVER_H
