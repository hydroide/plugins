QT             += widgets

TARGET          = $$qtLibraryTarget(testplugin)
TEMPLATE        = lib
CONFIG         += plugin

DESTDIR         = ../../plugins

INCLUDEPATH    += ../../interfaces

HEADERS         = testplugin.h
SOURCES         = testplugin.cpp
DISTFILES      += testplugin.json

include(../../interfaces/interfaces.pri)
