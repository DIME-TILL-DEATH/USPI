#include "plugininfo.h"

PluginInfo::PluginInfo(QString name, QString path, QString description, QMap<QString, QString> settings)
    : m_name{name},
      m_path{path},
      m_description{description},
      m_settings{settings}
{

}

const QString &PluginInfo::name() const
{
    return m_name;
}

const QString &PluginInfo::path() const
{
    return m_path;
}

const QString &PluginInfo::description() const
{
    return m_description;
}

const QMap<QString, QString> &PluginInfo::settings() const
{
    return m_settings;
}

void PluginInfo::setSettings(const QMap<QString, QString> &newSettings)
{
    m_settings = newSettings;
}
