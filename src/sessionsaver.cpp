#include "sessionsaver.h"
#include "JsonWorker.h"

SessionSaver::SessionSaver(DUTDevice *device, ExtensionManager *extensionManager, std::vector<std::shared_ptr<Register> >* localRegisterMap, RegisterListModel *registerMapModel, RegisterListModel *registerWriteSequenceModel)
    :m_device{device},
     m_extensionManager{extensionManager},
     m_localRegisterMap{localRegisterMap},
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

        saver.saveHeader(m_device->deviceHeader());
        saver.saveRegisterArray(m_device->deviceRegisterMap());

        jsonGlobalObject["DUT"] = saver.deviceGlobalObject();

        jsonGlobalObject["project settings"] = saveProjectSettings();
        jsonGlobalObject["plugins"] = savePlugInsSettings();
        jsonGlobalObject["write sequence"] = saveWriteSequence();

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
            qWarning() << QObject::tr("Ошибка в файле json: ") + jsonError.errorString() +QObject::tr(" в позиции ") + QString::number(jsonError.offset, 10);
            return false;
        }

        QJsonObject globalObject = jsonDoc.object();

        loadDUT(globalObject);
        loadWriteSequence(globalObject);
        loadProjectSettings(globalObject);
        loadPlugInSettings(globalObject);

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

bool SessionSaver::loadDUT(QJsonObject globalObject)
{
    if(globalObject.contains("DUT") && globalObject["DUT"].isObject())
    {
        QJsonObject jsonDUTobject = globalObject["DUT"].toObject();

        ParseError error;
        if(!m_device->loadFromJsonObject(jsonDUTobject, &error))
        {
            qWarning() << error.errorString();
            return false;
        }
        m_registerMapModel->resetModel(m_device->deviceRegisterMap());
    }
    return true;
}

QJsonArray SessionSaver::saveWriteSequence()
{
    QJsonArray jsonWriteSequenceArray;

    int index =0;
    foreach(RegisterAdapter currentAdapter, m_registerWriteSequenceModel->registerAdaptersList())
    {
        QJsonObject jsonItem;

        jsonItem["order"] = index;
        jsonItem["SourceUniqueID"] = currentAdapter.getRegister()->uniqueId();

        if(currentAdapter.isLocal())
        {
            QJsonObject jsonLocalRegisterData;
            JsonWorker::saveRegister(*currentAdapter.getRegister(), jsonLocalRegisterData);

            jsonItem["local"] = jsonLocalRegisterData;
        }

        if(currentAdapter.getRegister()->registerType() == RegisterType::Controller)
        {
            QJsonObject jsonLocalRegisterData;
            JsonWorker::saveRegister(*currentAdapter.getRegister(), jsonLocalRegisterData);

            jsonItem["controller"] = jsonLocalRegisterData;
        }

        jsonWriteSequenceArray.append(jsonItem);
        index++;
    }
    return jsonWriteSequenceArray;
}

bool SessionSaver::loadWriteSequence(QJsonObject globalObject)
{
    if(globalObject.contains("write sequence") && globalObject["write sequence"].isArray())
    {
        QJsonArray sequnceArray = globalObject["write sequence"].toArray();

        for (int registerIndex = 0; registerIndex < sequnceArray.size(); ++registerIndex)
        {
            QJsonObject variantObject = sequnceArray[registerIndex].toObject();
            if(variantObject.contains("SourceUniqueID"))
            {
                quint16 uniqueId= variantObject["SourceUniqueID"].toInt();
                RegisterAdapter* adapter = m_registerMapModel->getRegisterAdapterByUniqueId(uniqueId);
                m_registerWriteSequenceModel->addItem(*adapter, registerIndex);

                if(variantObject.contains("local"))
                {
                    QJsonObject jsonLocalRegisterObject = variantObject["local"].toObject();

                    Register* reg_ptr = new Register();

                    JsonWorker::readRegister(jsonLocalRegisterObject, reg_ptr);
                    m_localRegisterMap->push_back(std::shared_ptr<Register>(reg_ptr));

                    RegisterAdapter adapter(m_localRegisterMap->back());
                    adapter.setIsLocal(true);

                    m_registerWriteSequenceModel->changeItem(adapter, registerIndex);
                }

                if(variantObject.contains("controller"))
                {
                    QJsonObject jsonLocalRegisterObject = variantObject["controller"].toObject();

                    Register* reg_ptr = new Register();

                    JsonWorker::readRegister(jsonLocalRegisterObject, reg_ptr);
                    m_localRegisterMap->push_back(std::shared_ptr<Register>(reg_ptr));

                    RegisterAdapter adapter(m_localRegisterMap->back());
                    adapter.setIsLocal(true);

                    m_registerWriteSequenceModel->changeItem(adapter, registerIndex);
                }
            }
        }
        return true;
    }
    else return false;
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

QJsonArray SessionSaver::savePlugInsSettings()
{
    QJsonArray plugInSettingsObject;

    JsonWorker::savePlugInsArray(m_extensionManager->loadedPlugInsInfo(), plugInSettingsObject);

    return plugInSettingsObject;
}

bool SessionSaver::loadPlugInSettings(QJsonObject globalObject)
{
    std::vector<PluginInfo> plugList;

    if(!JsonWorker::readPluginsArray(globalObject ,&plugList)) return false;
    m_extensionManager->unloadPlugins();
    m_extensionManager->loadPlugins(plugList);

    return true;
}

