#-------------------------------------------------
#
# Project created by QtCreator 2019-05-07T15:06:51
#
#-------------------------------------------------

QT       += core gui

TARGET = projectmanager
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/generic

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    projectmanagerplugin.cpp

HEADERS += \
    projectmanagerplugin.h

DISTFILES += projectmanager.json 

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../commons/release/ -lcommons
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../commons/debug/ -lcommons
else:unix: LIBS += -L$$OUT_PWD/../../../commons/ -lcommons

INCLUDEPATH += $$PWD/../../../commons
DEPENDPATH += $$PWD/../../../commons

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../commons/release/libcommons.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../commons/debug/libcommons.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../commons/release/commons.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../commons/debug/commons.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../../commons/libcommons.a
