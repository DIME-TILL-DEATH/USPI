#ifndef PLUGININFO_H
#define PLUGININFO_H

#include <QString>
#include <QMap>

class PluginInfo
{
public:
    PluginInfo(QString name = "undefined", QString path = "", QString description = "", QMap<QString, QString> settings = {});

    const QString &name() const;
    const QString &path() const;
    const QString &description() const;

    const QMap<QString, QString> &settings() const;
    void setSettings(const QMap<QString, QString> &newSettings);

private:
    QString m_name;
    QString m_path;
    QString m_description;

    QMap<QString, QString> m_settings;
};

#endif // PLUGININFO_H
