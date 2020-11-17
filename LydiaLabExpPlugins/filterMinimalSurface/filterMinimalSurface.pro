include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
StarlabTemplate(plugin)

SOURCES += \
    FilterMinimalSurfacePlugin.cpp

HEADERS += \
    FilterMinimalSurfacePlugin.h
