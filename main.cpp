#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QFile>
#include <QDir>

#include <QDebug>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "src/fileparser.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    //---------------------------------------------------------------
    FileParser jsonFile;

    DUTDevice device("MT.json");
    jsonFile.loadFile("MT.json");
    jsonFile.readHeader(&device.m_deviceHeader);
    jsonFile.readRegisterArray(&device.m_deviceRegisterMap);


    //---------------------------------------------------------------

    QQmlApplicationEngine engine;

    engine.addImportPath(":/qml");
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
