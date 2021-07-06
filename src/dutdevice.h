#ifndef DUTDEVICE_H
#define DUTDEVICE_H

#include <QString>

#include "register.h"

//class Register;

class DUTDevice
{
public:
    DUTDevice(const QString& fileName);

    static bool fromFile(const QString& fileName);

    struct Header{
        QString deviceName;
        QString version;
        quint8 registerSize;
        bool  isMSB;
    };
    Header m_deviceHeader;
    std::vector<Register> m_deviceRegisterMap;
private:
};

#endif // DUTDEVICE_H
