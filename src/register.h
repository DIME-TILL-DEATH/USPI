#ifndef REGISTER_H
#define REGISTER_H

#include <vector>
#include <QString>
#include <QHash>
#include <QDataStream>
#include <QDebug>

#include "abstractfield.h"
//#include "registeradapter.h"

#include "bitfield.h"
#include "fixedfield.h"
#include "integerfield.h"
#include "variantlistfield.h"
#include "separationfield.h"

//#include "dutdevice.h"

//#include "fieldadapter.h"
#include "parseerror.h"

class JsonWorker;
struct DUTHeader;

enum class RegisterType
{
    DUT=0x00,
    Controller=0x01
};

class Register
{
public:

    friend class JsonWorker;

    Register(DUTHeader *parentDUTHeader = nullptr);
    ~Register();

    Register(const Register& src_reg); // copy
//    Register(Register&& src_reg) noexcept; //move

    QString name() const;
    quint16 fieldsCount();

    const DUTHeader* parentDUTHeader() const;

    bool sortAndValidateFields(ParseError* error = nullptr);

    // наверное надо привести к общему виду
    AbstractField &field(quint16 fieldIndex);
    AbstractField *field(QString fieldName);

    QList<QByteArray> rawData();

    friend QDataStream& operator<<(QDataStream& stream, const Register& reg);
    friend QDataStream& operator>>(QDataStream& stream, Register& reg);

    quint16 uniqueId() const;

    quint16 bitSize() const;

    RegisterType registerType() const;

private:
    DUTHeader* m_parentDUTHeader;

    quint16 m_uniqueId;

    QString m_name{"Undefined"};
    quint16 m_bitSize;

    RegisterType m_registerType{RegisterType::DUT};

    // shared pointer?
    std::vector<AbstractField*> m_fields;

    bool validateSize(ParseError* error);
    bool validateBounds(ParseError* error);
};

bool operator==(const Register& lr, const Register& rr);

#endif // REGISTER_H
