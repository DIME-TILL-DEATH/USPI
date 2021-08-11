#include <QTime>

#include "logger.h"

void Logger::messageOutputHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    {
        if(currentHandler)
            currentHandler->messageOutputHandlerImplementation(type, context, msg);
    }
}

void Logger::setAsMessageHandlerForApp()
{
    currentHandler = this;
    qInstallMessageHandler(messageOutputHandler);
}

QHash<int, QByteArray> Logger::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ListRoles::MsgText] = "msgText";
    roles[ListRoles::MsgColor] = "msgColor";
    return roles;
}

int Logger::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(m_data.size());
}

QVariant Logger::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() > rowCount(index))
    {
        return {};
    }

    switch(role)
    {
        case ListRoles::MsgText:
        {
            return QVariant::fromValue(m_data.at(index.row()).text);
        }
        case ListRoles::MsgColor:
        {
            return QVariant::fromValue(m_data.at(index.row()).color);
        }
        default:
        {
            return true;
        }
    }
}

bool Logger::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row+count-1);
    m_data.insert(m_data.begin()+row, count, LogMessage());
    endInsertRows();
    return true;
}

void Logger::message(const QString& line, QColor color)
{
    insertRows(m_data.size(), 1);

    LogMessage msg;
    msg.text = line;
    msg.color = color;
    m_data.at(m_data.size()-1) = msg;

    emit dataChanged(createIndex(0, 0), createIndex(m_data.size(), 0));
}

void Logger::messageOutputHandlerImplementation(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    {
        QByteArray localMsg = msg.toLocal8Bit();
        const char *file = context.file ? context.file : "";
        const char *function = context.function ? context.function : "";

        switch (type)
        {
            case QtDebugMsg:
                fprintf(stdout, "Debug: %s \n(%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
                std::cout << std::endl << std::flush;
                break;

            case QtInfoMsg:
                message("Лог (" + QTime::currentTime().toString() +"): " + msg);
//                std::cout << localMsg.constData() << std::flush;
                break;

            case QtWarningMsg:
                message("Ошибка (" + QTime::currentTime().toString() +"): " +  msg, Qt::red);

                std::cout << "Error: " << localMsg.constData() << std::endl << std::flush;
                break;

            case QtCriticalMsg:
                fprintf(stdout, "Critical: %s \n(%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
                std::cout << std::endl << std::flush;
                break;

            case QtFatalMsg:
                fprintf(stdout, "Fatal: %s \n(%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
                std::cout << std::endl << std::flush;
                break;
        }
    }
}
