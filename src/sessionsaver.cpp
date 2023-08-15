#include "sessionsaver.h"
#include "JsonWorker.h"

SessionSaver::SessionSaver(std::vector<std::shared_ptr<DUTDevice> > *dutList, ExtensionManager *extensionManager, std::vector<std::shared_ptr<Register> >* localRegisterMap, RegisterListModel *registerMapModel, RegisterListModel *registerWriteSequenceModel)
    :m_deviceList{dutList},
     m_extensionManager{extensionManager},
     m_regSequenceMap{localRegisterMap},
     m_registerMapModel{registerMapModel},
     m_registerWriteSequenceModel{registerWriteSequenceModel}
{

}

bool SessionSaver::saveSession(const QString &filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        JsonWorker saver;

        QJsonObject jsonGlobalObject;

        jsonGlobalObject["devices"] = JsonWorker::saveDutList(m_deviceList);
        jsonGlobalObject["project settings"] = saveProjectSettings();
        jsonGlobalObject["plugins"] = JsonWorker::savePlugInsArray(m_extensionManager->loadedPlugInsInfo());
        jsonGlobalObject["write sequence"] = JsonWorker::saveWriteSequence(m_registerWriteSequenceModel);

        file.write(QJsonDocument(jsonGlobalObject).toJson());
        file.close();

        qInfo() << QObject::tr("Сессия") << filePath << QObject::tr("сохранена");
        return true;
    }
    else
    {
        qInfo() << QObject::tr("Невозможно открыть файл ") << filePath;
        return false;
    }
}

bool SessionSaver::loadSession(const QString &filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray fileData = file.readAll();

        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(fileData, &jsonError));

        if(jsonDoc.isNull())
        {
            qWarning() << QObject::tr("Ошибка в файле json: ") + jsonError.errorString() + QObject::tr(" в позиции ") + QString::number(jsonError.offset, 10);
            return false;
        }

        QJsonObject globalObject = jsonDoc.object();

        QMap<qint16, DUTHeader *> deviceReferenceList;
        deviceReferenceList.insert(-1, &m_currentInterface->selectedController()->m_controllerHeader);

        JsonWorker::readDutList(globalObject, m_deviceList, &deviceReferenceList);
        if(m_deviceList == nullptr)
        {
            qWarning() << "Ошибка загрузки описаний устройств";
            return false;
        }

        JsonWorker::readWriteSequence(globalObject, deviceReferenceList, m_regSequenceMap, m_registerWriteSequenceModel);

        loadProjectSettings(globalObject);

        std::vector<PluginInfo> plugList;

        if(!JsonWorker::readPluginsArray(globalObject ,&plugList, &deviceReferenceList)) return false;
        m_extensionManager->unloadAllPlugins();
        m_extensionManager->loadPlugins(plugList);

        file.close();
        qInfo() << QObject::tr("Сессия") << filePath << QObject::tr("загружена. Версия формата: ") << m_compareVersion;
        return true;
    }
    else
    {
        qWarning() << QObject::tr("Невозможно открыть файл ") << filePath;
        return false;
    }
}

QJsonObject SessionSaver::saveProjectSettings()
{
    QJsonObject settingsObject;

    settingsObject["format version"] = m_sessionSaverVersion;

    return settingsObject;
}

bool SessionSaver::loadProjectSettings(QJsonObject globalObject)
{
    if(globalObject.contains("project settings") && globalObject["project settings"].isObject())
    {
        QJsonObject jsonSettingsObject = globalObject["project settings"].toObject();

        QString versionString;
        if(jsonSettingsObject.contains("format version") && jsonSettingsObject["format version"].isString())
        {
            versionString = jsonSettingsObject["format version"].toString();
            versionString.remove(0, 2);

            m_compareVersion = versionString.toFloat();
        }
        return true;
    }
    else return false;
}

void SessionSaver::setInterface(AbstractInterface *newCurrentInterface)
{
    m_currentInterface = newCurrentInterface;
}
