# starlib
include($$[STARLAB])
# SurfaceMesh
include($$[SURFACEMESH])
# eigen
include($$PWD/../depends/eigen.pri)

StarlabTemplate(plugin)

HEADERS += \
    FilterOffsetPlugin.h

SOURCES += \
    FilterOffsetPlugin.cpp
