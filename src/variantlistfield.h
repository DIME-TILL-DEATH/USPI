#ifndef VARIANTLISTFIELD_H
#define VARIANTLISTFIELD_H

#include <QHash>

#include "abstractfield.h"

class FileParser;
class VariantListField : public AbstractField
{
public:
    VariantListField();

    QByteArray rawData(quint16 targetRegisterSize);

    friend class FileParser;

private:
    QHash<QString, quint32> m_data;
    quint8 m_size;
};

#endif // VARIANTLISTFIELD_H
