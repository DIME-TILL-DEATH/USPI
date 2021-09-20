#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include <QDebug>

#include <functional>

#include "src/logger.h"
#include "src/dutdevice.h"
#include "src/interfacessettingsadapter.h"

#include "src/fieldadapter.h"
#include "src/userinterface.h"

Logger* Logger::currentHandler = nullptr;

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QHash <QString, AbstractInterface* > avaliableInterfaces;
    InterfacesSettingsAdapter interfacesSettings(&avaliableInterfaces);

    Logger log;

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    //---------------------------------------------------------------
    app.setOrganizationName("ВНИИРТ");
    app.setOrganizationDomain("vniirt.ru");
    app.setApplicationName("USPI");

    UserInterface ui(&avaliableInterfaces);

    QObject::connect(&ui, &UserInterface::avaliableInterfacesUpdated, &interfacesSettings, &InterfacesSettingsAdapter::usbInterfaceSettingsChanged);
    QObject::connect(&ui, &UserInterface::avaliableInterfacesUpdated, &interfacesSettings, &InterfacesSettingsAdapter::fileInterfaceSettings);

    log.setAsMessageHandlerForApp();

    engine.rootContext()->setContextProperty("Backend", &ui);
    engine.rootContext()->setContextProperty("InterfaceSettings", &interfacesSettings);
    engine.rootContext()->setContextProperty("Log", &log);
    engine.rootContext()->setContextProperty("RegisterMapModel", ui.registerMapModel());
    engine.rootContext()->setContextProperty("RegisterSequenceModel", ui.registerSequenceModel());

    RegisterAdapter::registerTypes();
    FieldAdapter::registerTypes();
    InterfacesSettingsAdapter::registerTypes();
    UserInterface::registerTypes();
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
