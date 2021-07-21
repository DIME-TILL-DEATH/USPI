#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#include <QAbstractListModel>
#include <QColor>

class Logger: public QAbstractListModel
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = nullptr) {Q_UNUSED(parent)};

    static void messageOutputHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void setAsMessageHandlerForApp();

    struct LogMessage{
        QString text{""};
        QColor color{Qt::black};
    };

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex& index = {}, int role = Qt::DisplayRole) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    Q_INVOKABLE void message(const QString &line, QColor color = Qt::black);
private:
    std::vector<LogMessage> m_data;

    enum ListRoles{
        MsgText= Qt::UserRole + 1,
        MsgColor
    };

    static Logger* currentHandler;
    void messageOutputHandlerImplementation(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

#endif // LOGGER_H
