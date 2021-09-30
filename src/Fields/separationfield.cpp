#include "separationfield.h"

SeparationField::SeparationField()
{
    m_type = AbstractField::FieldType::SeparationField;
    m_size = 0;
}

SeparationField::SeparationField(AbstractField field)
        : AbstractField(field)
{
    m_type = AbstractField::FieldType::SeparationField;
    m_size = 0;
}
