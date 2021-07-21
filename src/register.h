#ifndef REGISTER_H
#define REGISTER_H

#include <vector>
#include <QString>
#include <QDataStream>

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

    friend QDataStream& operator<<(QDataStream& stream, const Register& reg);
    friend QDataStream& operator>>(QDataStream& stream, Register& reg);

    quint16 uniqueId() const;

private:
    quint16 m_uniqueId;

    QString m_name;
    quint16 m_bitSize;

    std::vector<AbstractField*> m_fields;

    bool validateSize(ParseError* error);
    bool validateBounds(ParseError* error);
    void sort();
};

#endif // REGISTER_H
