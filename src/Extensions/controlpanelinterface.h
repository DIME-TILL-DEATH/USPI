#ifndef CONTROLPANELINTERFACE_H
#define CONTROLPANELINTERFACE_H

#include <QString>
#include <QtPlugin>

class ControlPanelInterface
{
public:
    virtual void showPanel() = 0;
    virtual void closePanel() = 0;

signals:
    virtual void setFieldValue(QString registerName, QString fieldName, QVariant value) = 0;
    virtual void getFieldValue(QString registerName, QString fieldName, QVariant& value) = 0;

    virtual void writeRegisterSequence(QStringList registerNames) = 0;
    virtual void writeRegisterSequence() = 0;
public slots:
    virtual void saveRequest(QString plugInName, QMap<QString, QString>& dataToSave) = 0;
    virtual void loadRequest(QString plugInName, const QMap<QString, QString>& dataToLoad) = 0;
};

Q_DECLARE_INTERFACE(ControlPanelInterface, "com.vniirt.controlpanel")

#endif // CONTROLPANELINTERFACE_H
