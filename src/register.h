#ifndef REGISTER_H
#define REGISTER_H

#include <vector>

#include "abstractfield.h"
#include "parseerror.h"

class FileParser;

class Register
{
public:
    Register();
    ~Register();

private:
    QString m_name;
    quint16 m_bitSize;
    std::vector<AbstractField*> m_fields;

    friend class FileParser;
};

#endif // REGISTER_H
