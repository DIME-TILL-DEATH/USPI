#ifndef ABSTRACTFIELD_H
#define ABSTRACTFIELD_H

#include <QByteArray>
#include <QString>

class FileParser;

class AbstractField
{
public:
    AbstractField();
    virtual ~AbstractField();

    virtual QByteArray rawData(quint16 targetRegisterSize);

    enum class FieldType
    {
        UnknownField,
        FixedField,
        BitField,
        IntegerField,
        VariantListField
    };

protected:
    QString m_name;
    QString m_description;
    QString m_comment;
    FieldType m_type;

    quint8 m_position;

    friend class FileParser;
};

#endif // ABSTRACTFIELD_H
