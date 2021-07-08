#include "logger.h"

Logger::Logger(QObject *parent)
{
}

void Logger::message(const QString& line)
{
    m_list.append(line);
    this->setStringList(m_list);
}
