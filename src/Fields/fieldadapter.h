#ifndef FIELDADAPTER_H
#define FIELDADAPTER_H

#include <QObject>
#include <QVariant>
#include <QDebug>

#include <memory>

#include "abstractfield.h"
#include "fixedfield.h"
#include "integerfield.h"
#include "bitfield.h"
#include "variantlistfield.h"

class FieldAdapter
{
    Q_GADGET

    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(QString comment READ comment)

    Q_PROPERTY(QString type READ type)

    Q_PROPERTY(quint8 position READ position)
    Q_PROPERTY(quint8 size READ size)

    Q_PROPERTY(QVariant value READ value WRITE setValue)
    Q_PROPERTY(QVariant valueFrom READ valueFrom)
    Q_PROPERTY(QVariant valueTo READ valueTo)

    Q_PROPERTY(QVariant variantList READ variantList)

    Q_PROPERTY(QVariant scaledValue READ scaledValue WRITE setScaledValue)

    Q_PROPERTY(bool isValidValue READ isValidValue)

    Q_PROPERTY(quint16 viewOptions READ viewOptions WRITE setViewOptions)
public:
    FieldAdapter() {};
    FieldAdapter(AbstractField* field);
    ~FieldAdapter() = default;

    static void registerTypes();

    QString name() const;
    QString description() const;
    QString comment() const;

    QString type() const;

    quint16 position() const;
    quint16 size() const;

    QVariant value() const;
    void setValue(const QVariant& newValue);

    QVariant valueFrom() const;
    QVariant valueTo() const;

    QVariant variantList() const;

    QVariant scaledValue();
    void setScaledValue(const QVariant& newScaledValue);

    bool isValidValue();

    quint16 viewOptions() const;
    void setViewOptions(quint16 newOptions);

private:
    // надо везде проверять что m_field != nullptr
    // переделать на shared ptr?
    AbstractField* m_field;

    std::shared_ptr<quint16> m_viewOptions{nullptr};
};
Q_DECLARE_METATYPE(FieldAdapter)

#endif // FIELDADAPTER_H
