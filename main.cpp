#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include <QDir>

#include <QDebug>

#include <functional>

#include "logger.h"
#include "dutdevice.h"
#include "interfacessettingsadapter.h"

//#include "fieldadapter.h"
#include "userinterface.h"
#include "extensionmanager.h"

Logger* Logger::currentHandler = nullptr;

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QHash <QString, AbstractInterface* > avaliableInterfaces;
    InterfacesSettingsAdapter interfacesSettings(&avaliableInterfaces);

    Logger log;

//    QGuiApplication app(argc, argv);
    QApplication app(argc, argv);

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
    engine.rootContext()->setContextProperty("CurrentRegMapModel", ui.currentRegMapModel());
    engine.rootContext()->setContextProperty("RegisterSequenceModel", ui.registerSequenceModel());
    engine.rootContext()->setContextProperty("DutListModel", ui.dutListModel());

    RegisterAdapter::registerTypes();
    FieldAdapter::registerTypes();
    InterfacesSettingsAdapter::registerTypes();
    UserInterface::registerTypes();
    DUTDevice::registerTypes();

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
