#ifndef FIXEDFIELD_H
#define FIXEDFIELD_H

#include "abstractfield.h"

class FileParser;
class FixedField : public AbstractField
{
public:
    FixedField();

    QByteArray rawData(quint16 targetRegisterSize);

    friend class FileParser;
private:
    quint32 m_data;
};

#endif // FIXEDFIELD_H
