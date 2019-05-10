INCLUDEPATH    +=  $$PWD/../graphicswidgets

win32 {
    CONFIG(debug, release|debug): LIBSUBDIR = debug/
    CONFIG(release, release|debug): LIBSUBDIR = release/
}

LIBS += -L$$DEPTH/../graphicswidgets/$$LIBSUBDIR -lgraphicswidgets
