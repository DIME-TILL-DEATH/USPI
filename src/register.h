#ifndef REGISTER_H
#define REGISTER_H

#include <vector>
#include <QObject>

#include "abstractfield.h"

#include "fieldadapter.h"
#include "parseerror.h"

class FileParser;

class Register
{
    Q_GADGET
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(quint16 fieldsCount READ fieldsCount)

public:
    friend class FileParser;
    Register();
    ~Register();

    QString name() const;
    quint16 fieldsCount();

    bool sortAndValidateFields(ParseError* error = nullptr);

    Q_INVOKABLE FieldAdapter field(quint16 fieldIndex);

    Q_INVOKABLE QByteArray rawData();
private:
    QString m_name;
    quint16 m_bitSize;

    std::vector<AbstractField*> m_fields;

    FieldAdapter m_fieldAdapter;

};
Q_DECLARE_METATYPE(Register)

#endif // REGISTER_H
