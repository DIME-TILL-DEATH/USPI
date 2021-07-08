#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include <QDebug>

#include "src/logger.h"
#include "src/dutdevice.h"

#include "src/fieldadapter.h"
#include "src/userinterface.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    //---------------------------------------------------------------
    app.setOrganizationName("ВНИИРТ");
    app.setOrganizationDomain("VNIIRT");

    UserInterface ui;
    Logger log;

    engine.rootContext()->setContextProperty("Backend", &ui);
    engine.rootContext()->setContextProperty("Log", &log);

    qRegisterMetaType<Register>("Register");
    qRegisterMetaType<FieldAdapter>("FieldAdapter");
    //---------------------------------------------------------------

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
