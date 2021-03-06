#-------------------------------------------------
#
# Project created by QtCreator 2019-04-25T09:05:46
#
#-------------------------------------------------

QT       += core gui widgets

qtHaveModule(sql) {
    QT += sql
    DEFINES += QT_SQL
}

TARGET = z0gssb
TEMPLATE = lib
CONFIG += plugin


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    z0gssbplugin.cpp \
    ssbfile.cpp \
    z0gfile.cpp \
    zbfile.cpp

HEADERS += \
    z0gssbplugin.h \
    ssbfile.h \
    z0gfile.h \
    zbfile.h

DISTFILES += z0gssb.json 

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DEPTH = ../../
include($$DEPTH/plugin.pri)
DESTDIR = $$DEPTH/$$DESTDIR
include($$DEPTH/libcommons.pri)
