#ifndef ABSTRACTFIELD_H
#define ABSTRACTFIELD_H

#include <QByteArray>
#include <QString>
#include <QDataStream>

#include <math.h>

class JsonWorker;

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
        VariantListField,
        SeparationField
    };

    const QString &name() const;
    const QString &description() const;
    const QString &comment() const;

    FieldType type() const;

    quint16 position() const;
    quint16 size() const;

    friend QDataStream& operator<<(QDataStream& stream, const AbstractField& field);
    friend QDataStream& operator>>(QDataStream& stream, AbstractField& field);

protected:
    QString m_name;
    QString m_description;
    QString m_comment;
    FieldType m_type;

    quint16 m_position;
    quint16 m_size{1};

    QByteArray shiftBitData(quint64 data);

    friend class JsonWorker;
};

#endif // ABSTRACTFIELD_H
