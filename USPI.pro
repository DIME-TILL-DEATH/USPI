QT += quick
QT += quickcontrols2
QT += widgets

CONFIG += c++11
#CONFIG += console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    src/ControllerDevice/controllerdevice.cpp \
    src/ControllerDevice/devicepin.cpp \
        src/Fields/abstractfield.cpp \
        src/Fields/bitfield.cpp \
        src/Fields/fieldadapter.cpp \
        src/Fields/fixedfield.cpp \
        src/Fields/integerfield.cpp \
        src/Fields/variantlistfield.cpp \
        src/Interfaces/abstractinterface.cpp \
        src/Interfaces/fileinterface.cpp \
        src/Interfaces/usbinterface.cpp \
        src/dutdevice.cpp \
        src/fileparser.cpp \
        src/interfacessettingsadapter.cpp \
        src/logger.cpp \
        src/parseerror.cpp \
        src/register.cpp \
        src/registeradapter.cpp \
        src/registerlistmodel.cpp \
        src/sessionsaver.cpp \
        src/userinterface.cpp \


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_IMPORT_PATH += $$PWD/qml
QML2_IMPORT_PATH += $$PWD/qml

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/ControllerDevice/controllerdevice.h \
    src/ControllerDevice/devicepin.h \
    src/Fields/abstractfield.h \
    src/Fields/bitfield.h \
    src/Fields/fieldadapter.h \
    src/Fields/fixedfield.h \
    src/Fields/integerfield.h \
    src/Fields/variantlistfield.h \
    src/Interfaces/abstractinterface.h \
    src/Interfaces/fileinterface.h \
    src/Interfaces/usbinterface.h \
    src/dutdevice.h \
    src/fileparser.h \
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

INCLUDEPATH += $$PWD/libusb/include
LIBS += -L$$PWD/libusb/static -lusb-1.0
