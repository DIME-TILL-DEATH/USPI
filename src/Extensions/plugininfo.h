#ifndef PLUGININFO_H
#define PLUGININFO_H

#include <QString>

class PluginInfo
{
public:
    PluginInfo(QString name = "undefined", QString path = "", QString description = "");

    const QString &name() const;
    const QString &path() const;
    const QString &description() const;

private:
    QString m_name;
    QString m_path;
    QString m_description;
};

#endif // PLUGININFO_H
