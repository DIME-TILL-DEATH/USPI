#ifndef LOGGER_H
#define LOGGER_H

#include <QStringListModel>
#include <QStringList>

class Logger: public QStringListModel
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = nullptr);

    Q_INVOKABLE void message(const QString &line);
private:
    QStringList m_list;

};

#endif // LOGGER_H
