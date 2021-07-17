#ifndef INTERFACESSETTINGSADAPTER_H
#define INTERFACESSETTINGSADAPTER_H

#include <QObject>

#include "abstractinterface.h"
#include "fileinterface.h"

struct FileInterfaceSettings{

    Q_GADGET
public:
    QString m_filePath{"brrrr"};
    QString m_hexSeparator;
    QString m_binSeparator;

    Q_PROPERTY(QString filePath MEMBER m_filePath)
    Q_PROPERTY(QString hexSeparator MEMBER m_hexSeparator)
    Q_PROPERTY(QString binSeparator MEMBER m_binSeparator)
};
Q_DECLARE_METATYPE(FileInterfaceSettings)


class InterfacesSettingsAdapter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(FileInterfaceSettings fileInterfaceSettings READ fileInterfaceSettings WRITE setFileInterfaceSettings NOTIFY fileInterfaceSettingsChanged)
public:
    explicit InterfacesSettingsAdapter( QHash <QString, AbstractInterface* >* avaliableInterfaces, QObject *parent = nullptr);

    static void registerTypes();

    FileInterfaceSettings fileInterfaceSettings();
    void setFileInterfaceSettings(const FileInterfaceSettings& newSettings);

private:
     QHash <QString, AbstractInterface* >* m_avaliableInterfaces;

signals:
    void fileInterfaceSettingsChanged();
};

#endif // INTERFACESSETTINGSADAPTER_H
