#ifndef DUTDEVICE_H
#define DUTDEVICE_H

#include <QString>

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

private:
    Header m_deviceHeader;
    std::vector<Register> m_deviceRegisterMap{};
};

#endif // DUTDEVICE_H
