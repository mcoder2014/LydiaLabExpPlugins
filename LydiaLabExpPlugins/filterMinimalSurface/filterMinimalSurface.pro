include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
StarlabTemplate(plugin)

SOURCES += \
    FilterMinimalSurfacePlugin.cpp \
    LaplaceMinimalSurfaceTool.cpp

HEADERS += \
    FilterMinimalSurfacePlugin.h \
    LaplaceMinimalSurfaceTool.h
