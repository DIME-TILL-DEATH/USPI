#ifndef DUTDEVICE_H
#define DUTDEVICE_H

#include <QString>
#include <QDataStream>

#include <vector>
#include <memory>

#include "parseerror.h"
#include "register.h"
#include "registeradapter.h"

//class Register;

class DUTDevice
{
public:
    bool loadFromFile(const QString& fileName, ParseError* error = nullptr);
    bool loadFromJsonObject(const QJsonObject& jsonObject, ParseError* error = nullptr);

    struct Header{
        QString deviceName{QObject::tr("Выбрать")};
        QString version{""};
        quint8 registerSize{32};
        bool isMSB{true};
    };

    const QString& name() const;

    std::vector<std::shared_ptr<Register> > &deviceRegisterMap();
    const Header &deviceHeader() const;

    std::shared_ptr<Register> registerByUniqueId(quint16 uniqueId);
    std::shared_ptr<Register> registerByName(QString registerName);

    AbstractField* findField(QString registerName, QString fieldName);

    friend QDataStream& operator<<(QDataStream& stream, const DUTDevice& device);
    friend QDataStream& operator>>(QDataStream& stream, DUTDevice& device);
private:
    Header m_deviceHeader;

    std::vector<std::shared_ptr<Register> > m_deviceRegisterMap{};
};

#endif // DUTDEVICE_H
