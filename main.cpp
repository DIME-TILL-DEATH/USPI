#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include <QDebug>

#include "src/logger.h"
#include "src/dutdevice.h"
#include "src/interfacessettingsadapter.h"

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

    QHash <QString, AbstractInterface* > avaliableInterfaces;

    Logger log;
    InterfacesSettingsAdapter interfacesSettings(&avaliableInterfaces);
    UserInterface ui(&avaliableInterfaces, &log);

    engine.rootContext()->setContextProperty("Backend", &ui);
    engine.rootContext()->setContextProperty("InterfaceSettings", &interfacesSettings);
    engine.rootContext()->setContextProperty("Log", &log);
    engine.rootContext()->setContextProperty("RegisterMapModel", ui.registerMapModel());
    engine.rootContext()->setContextProperty("RegisterSequenceModel", ui.registerSequenceModel());

    qRegisterMetaType<RegisterAdapter>("RegisterAdapter");
    qRegisterMetaType<FieldAdapter>("FieldAdapter");

    InterfacesSettingsAdapter::registerTypes();
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
