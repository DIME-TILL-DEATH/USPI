#ifndef REGISTER_H
#define REGISTER_H

#include <vector>
#include <QString>

#include "abstractfield.h"
//#include "registeradapter.h"

#include "fieldadapter.h"
#include "parseerror.h"

class FileParser;

class Register
{
public:
    friend class FileParser;
    Register();
    ~Register();

    QString name() const;
    quint16 fieldsCount();

    bool sortAndValidateFields(ParseError* error = nullptr);

    AbstractField &field(quint16 fieldIndex);

    QByteArray rawData();

//    RegisterAdapter &adapter();

private:
    QString m_name;
    quint16 m_bitSize;

    std::vector<AbstractField*> m_fields;

    bool validateSize(ParseError* error);
    bool validateBounds(ParseError* error);
    void sort();
};

#endif // REGISTER_H
