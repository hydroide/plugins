QT             -= gui

TARGET          = $$qtLibraryTarget(testplugin)
TEMPLATE        = lib
CONFIG         += plugin

DESTDIR         = ../../plugins

HEADERS         = testplugin.h
SOURCES         = testplugin.cpp
DISTFILES      += testplugin.json

DEPTH = ../
include($$DEPTH/plugin.pri)
DESTDIR = $$DEPTH/$$DESTDIR
include($$DEPTH/libcommons.pri)
