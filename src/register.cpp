#include "register.h"

Register::Register()
{

}

Register::~Register()
{
}

quint16 Register::fieldsCount()
{
    return m_fields.size();
}

bool Register::sortAndValidateFields(ParseError *error)
{
    // checking bounds
    for(auto it_checker = m_fields.begin(); it_checker != m_fields.end()-1; ++it_checker )
    {
        quint16 startChecker = (*it_checker)->position();
        quint16 stopChecker = (*it_checker)->position() + (*it_checker)->size() - 1;

        for(auto it_verify = it_checker+1; it_verify != m_fields.end(); ++it_verify )
        {
            quint16 startVerify = (*it_verify)->position();
            quint16 stopVerify = (*it_verify)->position() + (*it_checker)->size() - 1;
            if((startChecker > stopVerify) || (stopChecker < startVerify) )
            {
             //ok
            }
            else
            {
                if(error != nullptr) error->setErrorType(ParseError::ErrorType::FieldsOverLaps, (*it_checker)->name() + " and " + (*it_verify)->name());
                return false;
            }
        }
    }

    // sorting
    AbstractField* temp;
    for(quint16 i=0; i < m_fields.size()-1; i++)
    {
        for(quint16 j=0; j< m_fields.size()-i-1; j++)
        {
            if(m_fields.at(j)->position() > m_fields.at(j+1)->position())
            {
                temp = m_fields.at(j);
                m_fields.at(j) = m_fields.at(j+1);
                m_fields.at(j+1) = temp;
            }
        }
    }
    return true;
}

FieldAdapter Register::field(quint16 fieldIndex)
{
    return FieldAdapter(m_fields.at(fieldIndex));
}

QString Register::name() const
{
    return m_name;
}
