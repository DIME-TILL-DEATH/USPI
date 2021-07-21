#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#include <QStringListModel>
#include <QStringList>

class Logger: public QStringListModel
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = nullptr) {Q_UNUSED(parent)};

    static void messageOutputHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void setAsMessageHandlerForApp();


    Q_INVOKABLE void message(const QString &line);
private:
    QStringList m_list;

    static Logger* currentHandler;
    void messageOutputHandlerImplementation(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

#endif // LOGGER_H
