QT             -= gui

TARGET          = $$qtLibraryTarget(testplugin)
TEMPLATE        = lib
CONFIG         += plugin

DESTDIR         = ../../plugins

HEADERS         = testplugin.h
SOURCES         = testplugin.cpp
DISTFILES      += testplugin.json


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../commons/release/ -lcommons
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../commons/debug/ -lcommons
else:unix: LIBS += -L$$OUT_PWD/../../commons/ -lcommons

INCLUDEPATH += $$PWD/../../commons
DEPENDPATH += $$PWD/../../commons

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../commons/release/libcommons.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../commons/debug/libcommons.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../commons/release/commons.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../commons/debug/commons.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../commons/libcommons.a
