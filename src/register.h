#ifndef REGISTER_H
#define REGISTER_H

#include <vector>
#include <QString>

#include "abstractfield.h"

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

    FieldAdapter field(quint16 fieldIndex);

    QByteArray rawData();
private:
    QString m_name;
    quint16 m_bitSize;

    std::vector<AbstractField*> m_fields;

    FieldAdapter m_fieldAdapter;

};

#endif // REGISTER_H
