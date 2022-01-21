#ifndef EXTENSIONMANAGER_H
#define EXTENSIONMANAGER_H

#include <QObject>
#include <QPluginLoader>

#include "dutdevice.h"
#include "plugininfo.h"

class ExtensionManager : public QObject
{
    Q_OBJECT
public:
    ExtensionManager(DUTDevice* controllingDevice, QObject* parent = nullptr);
    ~ExtensionManager();

    QStringList getPlugins(QString path);
    QStringList avaliablePlugInsNames();
    void loadPlugins(std::vector<PluginInfo> pluginsList);
    std::vector<PluginInfo> loadedPlugInsInfo();
    void loadPlugin(PluginInfo pluginIngo);

    void unloadPlugins();

    void runPlugin(QString pluginName);


private:
    QMap<QString, QPluginLoader*> m_pluginsList;
    std::vector<PluginInfo> m_loadedPlugInsInfo;

    QMap<QString, QString> getPlugInSettings(const QString& plugInName);
    void setPlugInSettings(const QString& plugInName, const QMap<QString, QString>& plugInSettings);

    DUTDevice* m_controlledDevice;

signals:
    void saveRequest(QString plugInName, QMap<QString, QString>& dataToSave);
    void loadRequest(QString plugInName, const QMap<QString, QString>& dataToLoad);

    void writeRegisterSequenceRequest(QStringList registerNames);

public slots:
    void setFieldValue(QString registerName, QString fieldName, QVariant value);
    void getFieldValue(QString registerName, QString fieldName, QVariant& value);

    void writeRegisterSequence(QStringList registerNames);
};

#endif // EXTENSIONMANAGER_H
