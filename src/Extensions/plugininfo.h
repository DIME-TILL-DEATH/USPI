#ifndef PLUGININFO_H
#define PLUGININFO_H

#include <QString>
#include <QMap>

#include "dutdevice.h"
#include "controlpanelinterface.h"

class PluginInfo
{
public:
    PluginInfo(QString name = "undefined", QString path = "", QString description = "", QMap<QString, QString> settings = {});

    const QString &name() const;
    const QString &path() const;
    const QString &description() const;

    const QMap<QString, QString> &settings() const;
    void setSettings(const QMap<QString, QString> &newSettings);

    DUTDevice *targetDevice();
    void setTargetDevice(DUTDevice *newTargetDevice);

    QPluginLoader *loader();
    void setLoader(QPluginLoader *newLoader);
private:
    DUTDevice* m_targetDevice;
    QPluginLoader* m_loader;

    QString m_name;
    QString m_path;
    QString m_description;

    QMap<QString, QString> m_settings;
};

#endif // PLUGININFO_H
