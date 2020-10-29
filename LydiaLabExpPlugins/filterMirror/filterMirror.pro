include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
StarlabTemplate(plugin)


SOURCES += \
    FilterMirrorPlugin.cpp \
    MirrorTools.cpp

HEADERS += \
    FilterMirrorPlugin.h \
    MirrorTools.h
