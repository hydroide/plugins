INCLUDEPATH    +=  $$PWD/../commons

win32 {
    CONFIG(debug, release|debug): LIBSUBDIR = debug/
    CONFIG(release, release|debug): LIBSUBDIR = release/
}

LIBS += -L$$DEPTH/../commons/$$LIBSUBDIR -lcommons
