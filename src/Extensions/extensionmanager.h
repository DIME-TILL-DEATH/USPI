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
    QStringList avaliablePluginsNames();
    void loadPlugins(std::vector<PluginInfo> pluginsList);
    std::vector<PluginInfo> loadedPlugInsInfo();
    void loadPlugin(PluginInfo pluginIngo);

    void unloadPlugins();

    void runPlugin(QString pluginName);
private:
    QMap<QString, QPluginLoader*> m_pluginsList;
    std::vector<PluginInfo> m_loadedPlugInsInfo;

    DUTDevice* m_controlledDevice;

public slots:
    void setFieldValue(QString registerName, QString fieldName, QVariant value);
    void getFieldValue(QString registerName, QString fieldName, QVariant& value);

    void writeRegisterSequence(QStringList registerNames);
};

#endif // EXTENSIONMANAGER_H
