QT += quick
QT += quickcontrols2
QT += widgets

CONFIG += c++11


VERSION = 0.5.0.3

SOURCES += \
        main.cpp \
        src/Controllers/abstractcontroller.cpp \
        src/Controllers/devicepin.cpp \
        src/Controllers/usbcontroller.cpp \
        src/Extensions/controlpanelinterface.cpp \
        src/Extensions/extensionmanager.cpp \
        src/Extensions/plugininfo.cpp \
        src/Fields/abstractfield.cpp \
        src/Fields/bitfield.cpp \
        src/Fields/fieldadapter.cpp \
        src/Fields/fixedfield.cpp \
        src/Fields/integerfield.cpp \
        src/Fields/variantlistfield.cpp \
        src/Fields/separationfield.cpp \
        src/Interfaces/abstractinterface.cpp \
    src/Interfaces/ethernetinterface.cpp \
        src/Interfaces/fileinterface.cpp \
    src/Interfaces/interfacemanager.cpp \
    src/Interfaces/interfaceslistmodel.cpp \
        src/Interfaces/usbinterface.cpp \
        src/JsonWorker.cpp \
        src/dutdevice.cpp \
    src/dutlistmodel.cpp \
        src/interfacessettingsadapter.cpp \
        src/logger.cpp \
        src/parseerror.cpp \
        src/register.cpp \
        src/registeradapter.cpp \
        src/registerlistmodel.cpp \
        src/sessionsaver.cpp \
        src/userinterface.cpp \


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_IMPORT_PATH += $$PWD/qml
QML2_IMPORT_PATH += $$PWD/qml

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/Controllers/abstractcontroller.h \
    src/Controllers/devicepin.h \
    src/Controllers/usbcontroller.h \
    src/Extensions/controlpanelinterface.h \
    src/Extensions/extensionmanager.h \
    src/Extensions/plugininfo.h \
    src/Fields/abstractfield.h \
    src/Fields/bitfield.h \
    src/Fields/fieldadapter.h \
    src/Fields/fixedfield.h \
    src/Fields/integerfield.h \
    src/Fields/variantlistfield.h \
    src/Fields/separationfield.h \
    src/Interfaces/abstractinterface.h \
    src/Interfaces/ethernetinterface.h \
    src/Interfaces/fileinterface.h \
    src/Interfaces/interfacemanager.h \
    src/Interfaces/interfacenames.h \
    src/Interfaces/interfaceslistmodel.h \
    src/Interfaces/usbinterface.h \
    src/JsonWorker.h \
    src/dutdevice.h \
    src/dutlistmodel.h \
    src/interfacessettingsadapter.h \
    src/logger.h \
    src/parseerror.h \
    src/register.h \
    src/registeradapter.h \
    src/registerlistmodel.h \
    src/sessionsaver.h \
    src/userinterface.h \

INCLUDEPATH += src/
INCLUDEPATH += src/Fields/
INCLUDEPATH += src/Interfaces/
INCLUDEPATH += src/Controllers/
INCLUDEPATH += src/Extensions/

INCLUDEPATH += $$PWD/libusb/include
LIBS += -L$$PWD/libusb/static -lusb-1.0

CONFIG(release, debug|release) {
    QMAKE_POST_LINK += windeployqt.exe  release/$${TARGET}.exe -qmldir=$${PWD}/qml/ --dir deploy $$escape_expand(\\n\\t)# $$RETURN

    file = release/$${TARGET}.exe
    dir = deploy
    # replace slashes in source path for Windows
    win32:file ~= s,/,\\,g
    # replace slashes in destination path for Windows
    win32:dir ~= s,/,\\,g

    QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($$file) $$shell_quote($$dir) $$escape_expand(\\n\\t)
}
