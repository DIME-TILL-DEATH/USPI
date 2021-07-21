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

void Logger::message(const QString& line)
{
    m_list.append(line);
    this->setStringList(m_list);
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
                std::cout << std::flush;
                break;

            case QtInfoMsg:
                message("Лог: " + msg);
                break;
            case QtWarningMsg:
                message("Ошибка: " + msg);
                break;

            case QtCriticalMsg:
                fprintf(stdout, "Critical: %s \n(%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
                std::cout << std::flush;
                break;
            case QtFatalMsg:
                fprintf(stdout, "Fatal: %s \n(%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
                std::cout << std::flush;
                break;
        }
    }
}
