#-------------------------------------------------
#
# Project created by QtCreator 2019-04-29T14:17:31
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = processviewer
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../plugins

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
    processviewer.cpp \
    processviewerplugin.cpp

HEADERS += \
    processviewer.h \
    processviewerplugin.h

DISTFILES += processviewer.json 

unix {
    target.path = /usr/lib
    INSTALLS += target
}

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../graphicswidgets/release/ -lgraphicswidgets
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../graphicswidgets/debug/ -lgraphicswidgets
else:unix: LIBS += -L$$OUT_PWD/../../graphicswidgets/ -lgraphicswidgets

INCLUDEPATH += $$PWD/../../graphicswidgets
DEPENDPATH += $$PWD/../../graphicswidgets

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../graphicswidgets/release/libgraphicswidgets.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../graphicswidgets/debug/libgraphicswidgets.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../graphicswidgets/release/graphicswidgets.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../graphicswidgets/debug/graphicswidgets.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../graphicswidgets/libgraphicswidgets.a
