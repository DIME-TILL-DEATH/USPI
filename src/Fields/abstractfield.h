#ifndef ABSTRACTFIELD_H
#define ABSTRACTFIELD_H

#include <QByteArray>
#include <QString>
#include <QDataStream>

#include <math.h>

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

    const QString &name() const;
    const QString &description() const;
    const QString &comment() const;

    FieldType type() const;

    quint8 position() const;
    quint8 size() const;

    friend QDataStream& operator<<(QDataStream& stream, const AbstractField& field);
    friend QDataStream& operator>>(QDataStream& stream, AbstractField& field);

protected:
    QString m_name;
    QString m_description;
    QString m_comment;
    FieldType m_type;

    quint8 m_position;
    quint8 m_size{1};

    QByteArray shiftBitData(quint64 data);

    friend class FileParser;
};

#endif // ABSTRACTFIELD_H
