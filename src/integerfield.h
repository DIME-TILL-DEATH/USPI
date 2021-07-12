#ifndef INTEGERFIELD_H
#define INTEGERFIELD_H

#include "abstractfield.h"

class FileParser;
class IntegerField : public AbstractField
{
public:
    friend class FileParser;

    IntegerField();

    QByteArray rawData(quint16 targetRegisterByteSize);

    quint64 data() const;
    void setData(quint64 newData);

    quint32 valueFrom() const;
    quint32 valueTo() const;

private:
    quint64 m_data;

    quint32 m_valueFrom;
    quint32 m_valueTo;
};

#endif // INTEGERFIELD_H
