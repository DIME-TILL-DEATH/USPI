#ifndef FIXEDFIELD_H
#define FIXEDFIELD_H

#include <QDataStream>

#include "abstractfield.h"

class JsonWorker;
class FixedField : public AbstractField
{
public:
    FixedField() {m_type = AbstractField::FieldType::FixedField;};
    FixedField(AbstractField field) : AbstractField{field} {m_type = AbstractField::FieldType::FixedField;};

    QByteArray rawData(quint16 targetRegisterByteSize);

    friend class JsonWorker;
    quint32 data() const;

    friend QDataStream& operator<<(QDataStream& stream, const FixedField& field);
    friend QDataStream& operator>>(QDataStream& stream, FixedField& field);
private:
    quint64 m_data;
};

#endif // FIXEDFIELD_H
