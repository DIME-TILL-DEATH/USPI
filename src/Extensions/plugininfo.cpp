#include "plugininfo.h"

PluginInfo::PluginInfo(QString name, QString path, QString description)
    : m_name{name},
      m_path{path},
      m_description{description}
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
