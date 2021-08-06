#ifndef INTERFACESSETTINGSADAPTER_H
#define INTERFACESSETTINGSADAPTER_H

#include <QObject>

#include "abstractinterface.h"
#include "fileinterface.h"
#include "usbinterface.h"

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

struct USBInterfaceSettings{
    Q_GADGET
public:
    QString deviceName;
    QStringList deviceInfo;

    Q_PROPERTY(QString deviceName MEMBER deviceName)
    Q_PROPERTY(QStringList deviceInfo MEMBER deviceInfo)
};
Q_DECLARE_METATYPE(USBInterfaceSettings)


class InterfacesSettingsAdapter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(FileInterfaceSettings fileInterfaceSettings READ fileInterfaceSettings WRITE setFileInterfaceSettings NOTIFY fileInterfaceSettingsChanged)
    Q_PROPERTY(USBInterfaceSettings usbInterfaceSettings READ usbInterfaceSettings WRITE setUSBInterfaceSettings NOTIFY usbInterfaceSettingsChanged)
public:
    explicit InterfacesSettingsAdapter( QHash <QString, AbstractInterface* >* avaliableInterfaces, QObject *parent = nullptr);

    static void registerTypes();

    FileInterfaceSettings fileInterfaceSettings();
    void setFileInterfaceSettings(const FileInterfaceSettings& newSettings);

    USBInterfaceSettings usbInterfaceSettings();
    void setUSBInterfaceSettings(const USBInterfaceSettings& newSettings);

private:
     QHash <QString, AbstractInterface* >* m_avaliableInterfaces;

signals:
    void fileInterfaceSettingsChanged();
    void usbInterfaceSettingsChanged();
};

#endif // INTERFACESSETTINGSADAPTER_H
