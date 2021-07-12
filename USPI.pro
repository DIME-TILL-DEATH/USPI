QT += quick
QT += quickcontrols2

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        src/abstractfield.cpp \
        src/abstractinterface.cpp \
        src/bitfield.cpp \
        src/dutdevice.cpp \
        src/fieldadapter.cpp \
        src/fileparser.cpp \
        src/fixedfield.cpp \
        src/integerfield.cpp \
        src/logger.cpp \
        src/parseerror.cpp \
        src/register.cpp \
        src/registeradapter.cpp \
        src/userinterface.cpp \
        src/variantlistfield.cpp

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
    src/abstractfield.h \
    src/abstractinterface.h \
    src/bitfield.h \
    src/dutdevice.h \
    src/fieldadapter.h \
    src/fileparser.h \
    src/fixedfield.h \
    src/integerfield.h \
    src/logger.h \
    src/parseerror.h \
    src/register.h \
    src/registeradapter.h \
    src/userinterface.h \
    src/variantlistfield.h
