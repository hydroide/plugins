TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets
INCLUDEPATH    += ../../interfaces
HEADERS         = testplugin.h
SOURCES         = testplugin.cpp
TARGET          = $$qtLibraryTarget(testplugin)
DESTDIR         = ../../plugins

include(../../interfaces/interfaces.pri)

EXAMPLE_FILES   = testplugin.json
