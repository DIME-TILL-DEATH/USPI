#include <QtQml>

#include <cmath>

#include "fieldadapter.h"


FieldAdapter::FieldAdapter(AbstractField *field)
    : m_field{field}
{
    m_viewOptions = std::make_shared<quint16>(quint16(10));
}

void FieldAdapter::registerTypes()
{
    qRegisterMetaType<FieldAdapter>("FieldAdapter");
//    qRegisterMetaType<FieldScale>("IntegerFieldScaler");
}

QString FieldAdapter::name() const
{
    return m_field->name();
}

QString FieldAdapter::description() const
{
    return m_field->description();
}

QString FieldAdapter::comment() const
{
    return m_field->comment();
}

QString FieldAdapter::type() const
{
    switch(m_field->type())
    {
        case AbstractField::FieldType::FixedField: return "fixed";
        case AbstractField::FieldType::BitField: return "bit";
        case AbstractField::FieldType::IntegerField: return "integer";
        case AbstractField::FieldType::VariantListField: return "variant_list";
        case AbstractField::FieldType::SeparationField: return "separator";
        default: return "unknown";
    }
}

quint16 FieldAdapter::position() const
{
    return m_field->position();
}

quint16 FieldAdapter::size() const
{
    return m_field->size();
}

QVariant FieldAdapter::value() const
{
    switch(m_field->type())
    {
        case AbstractField::FieldType::FixedField:
            return dynamic_cast<FixedField*>(m_field)->data();

        case AbstractField::FieldType::BitField:
            return dynamic_cast<BitField*>(m_field)->getBit();

        case AbstractField::FieldType::IntegerField:
        {
            QString result;
            result.setNum(dynamic_cast<IntegerField*>(m_field)->data(), *m_viewOptions);

            switch (*m_viewOptions)
            {
                case 2: result.prepend("0b"); break;
                case 16: result.prepend("0x"); break;
            }
            return result;
        }

        case AbstractField::FieldType::VariantListField:
            return dynamic_cast<VariantListField*>(m_field)->selected();
        return  "variant_list";

        default: return "Value undefined";
    }
}

void FieldAdapter::setValue(const QVariant &newValue)
{
    switch(m_field->type())
    {
        case AbstractField::FieldType::BitField:
            dynamic_cast<BitField*>(m_field)->setBit(newValue.toBool());
            break;
        case AbstractField::FieldType::IntegerField:
        {
            bool result;
            quint64 value;

            if(newValue.toString().startsWith("0b"))
            {
                QString convString = newValue.toString().remove(0, 2);
                value = convString.toULongLong(&result, 2);
            }
            else
            {
                value = newValue.toString().toULongLong(&result, 0);
            }

            if(result) dynamic_cast<IntegerField*>(m_field)->setData(value);
            break;
        }
        case AbstractField::FieldType::VariantListField:
            dynamic_cast<VariantListField*>(m_field)->setSelected(newValue.toString());
        default: break;
    }
}

QVariant FieldAdapter::valueFrom() const
{
    if(m_field->type() == AbstractField::FieldType::IntegerField)
    {
        return dynamic_cast<IntegerField*>(m_field)->valueFrom();
    }
    else
    {
        return 0;
    }
}

QVariant FieldAdapter::valueTo() const
{
    if(m_field->type() == AbstractField::FieldType::IntegerField)
    {
        return dynamic_cast<IntegerField*>(m_field)->valueTo();
    }
    else
    {
        return 0;
    }
}

QVariant FieldAdapter::variantList() const
{
    if(m_field->type() == AbstractField::FieldType::VariantListField)
    {
        QStringList result;
        VariantListField* field_ptr = dynamic_cast<VariantListField*>(m_field);

        for(auto it=field_ptr->data().begin(); it != field_ptr->data().end(); it++)
        {
            result.append(it.value()); // + " - 0x" + QString::number(it.key(), 16));

        }
        return result;
    }
    else
    {
        return value();
    }
}

QVariant FieldAdapter::scaledValue()
{
    if(m_field->type() == AbstractField::FieldType::IntegerField)
    {
        IntegerField* field_ptr = dynamic_cast<IntegerField*>(m_field);

        qreal K = field_ptr->scaleCoefficient();
        qreal exp = field_ptr->scaleExponent();
        qreal off1 = field_ptr->scaleOffset1();
        qreal off2 = field_ptr->scaleOffset2();

        if(K==1 && exp==1 && off1==0 && off2==0) return "none";

        qreal val = field_ptr->data();
        qreal resultValue = K*pow(val+off1, exp)+off2;

        QString resultString;
        resultString.setNum(resultValue);
        resultString.append(" " + field_ptr->scaleUnits());

        return resultString;
    }
    else
    {
        return value();
    }
}

void FieldAdapter::setScaledValue(const QVariant &newScaledValue)
{
    if(m_field->type() == AbstractField::FieldType::IntegerField)
    {
        IntegerField* field_ptr = dynamic_cast<IntegerField*>(m_field);

        qreal K = field_ptr->scaleCoefficient();
        qreal exp = field_ptr->scaleExponent();
        qreal off1 = field_ptr->scaleOffset1();
        qreal off2 = field_ptr->scaleOffset2();

        if(K==1 && exp==1 && off1==0 && off2==0)
        {

        }
        else
        {
            QString expression = newScaledValue.toString();

            QRegularExpression regExp("[0-9]*[.,]?[0-9]*");
            QRegularExpressionMatch match = regExp.match(expression);

            if(match.hasMatch())
            {
                QString word = match.captured(0);

                qreal val = word.toDouble();
                qreal resultValue = pow((val-off2)/K, 1/exp)-off1;
                field_ptr->setData(std::round(resultValue));
            }
        }
    }
}

bool FieldAdapter::isValidValue()
{
    switch(m_field->type())
    {
        case AbstractField::FieldType::IntegerField:
        {
            return dynamic_cast<IntegerField*>(m_field)->isValidValue();
            break;
        }
        default: return true;;
    }
}

quint16 FieldAdapter::viewOptions() const
{
    return *m_viewOptions;
}

void FieldAdapter::setViewOptions(quint16 newOptions)
{
    *m_viewOptions = newOptions;
}
