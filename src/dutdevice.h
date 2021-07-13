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
        QString deviceName;
        QString version;
        quint8 registerSize;
        bool  isMSB;
    };

    quint16 registerCount();
    RegisterAdapter registerAdapter(quint16 registerIndex);
private:
    Header m_deviceHeader;
    std::vector<Register> m_deviceRegisterMap;
};

#endif // DUTDEVICE_H
