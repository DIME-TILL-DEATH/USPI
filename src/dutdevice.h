#ifndef DUTDEVICE_H
#define DUTDEVICE_H

#include <QString>
#include <QDataStream>
#include <vector>

#include "parseerror.h"
#include "register.h"
#include "registeradapter.h"

//class Register;

class DUTDevice
{
public:
    bool loadFromFile(const QString& fileName, ParseError* error = nullptr);

    struct Header{
        QString deviceName{"Выбрать"};
        QString version{""};
        quint8 registerSize{32};
        bool isMSB{true};
    };

    const QString& name() const;

    std::vector<Register>& deviceRegisterMap();
    const Header &deviceHeader() const;

    Register* registerByUniqueId(quint16 uniqueId);

    friend QDataStream& operator<<(QDataStream& stream, const DUTDevice& device);
    friend QDataStream& operator>>(QDataStream& stream, DUTDevice& device);
private:
    Header m_deviceHeader;

    // RegisterAdapter хранит ссылку на элемент вектора.
    // Если вектор реаллоцируется ссылка протухнет!
    // Переделать на std::array, чтобы размещение в памяти было статичное и ссылки не протухали
    std::vector<Register> m_deviceRegisterMap{};
};

#endif // DUTDEVICE_H
