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
    ExtensionManager(QObject* parent = nullptr);
    ~ExtensionManager();

    QStringList getPlugins(const QString& path);
    QStringList avaliablePlugIns();
    void loadPlugins(const std::vector<PluginInfo>& pluginsList);
    std::vector<PluginInfo> loadedPlugInsInfo();
    void loadPlugin(PluginInfo &pluginInfo);

    void unloadPlugins();

//    void runPlugin(const QString& pluginName);
    void runPlugin(quint16 number);

private:
//    QMap<QString, QPluginLoader*> m_pluginsList;
    std::vector<PluginInfo> m_loadedPlugInsInfo;

    QMap<QString, QString> getPlugInSettings(const QString& plugInName);
    void setPlugInSettings(const QString& plugInName, const QMap<QString, QString>& plugInSettings);

signals:
    void saveRequest(QString plugInName, QMap<QString, QString>& dataToSave);
    void loadRequest(QString plugInName, const QMap<QString, QString>& dataToLoad);

    void writeCustomSequenceRequest(QStringList registerNames, DUTDevice *targetDevice);
    void writeSequenceRequest();

public slots:
    void setFieldValue(QString registerName, QString fieldName, QVariant value);
    void getFieldValue(QString registerName, QString fieldName, QVariant& value);

    void writeRegisterSequence(QStringList registerNames);
    void writeRegisterSequence();
};

#endif // EXTENSIONMANAGER_H
