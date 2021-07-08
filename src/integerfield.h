#ifndef INTEGERFIELD_H
#define INTEGERFIELD_H

#include "abstractfield.h"

class FileParser;
class IntegerField : public AbstractField
{
public:
    IntegerField();

    QByteArray rawData(quint16 targetRegisterSize);

    friend class FileParser;
private:
    quint64 m_data;

    quint32 m_valueFrom;
    quint32 m_valueTo;
};

#endif // INTEGERFIELD_H
