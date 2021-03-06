#-------------------------------------------------
#
# data created by QtCreator 2019-05-07T15:06:51
#
#-------------------------------------------------

QT       += core gui widgets sql

TARGET = datamanager
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../../plugins

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
    datamanagerplugin.cpp \
    dataeditor.cpp \
    datawindow.cpp \
    sqltablemodel.cpp

HEADERS += \
    datamanagerplugin.h \
    dataeditor.h \
    datawindow.h \
    sqltablemodel.h

FORMS += \
    datawindow.ui

DISTFILES += datamanager.json 

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DEPTH = ../../
include($$DEPTH/plugin.pri)
DESTDIR = $$DEPTH/$$DESTDIR
include($$DEPTH/libcommons.pri)
