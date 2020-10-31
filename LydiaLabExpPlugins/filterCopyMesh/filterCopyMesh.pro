include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
StarlabTemplate(plugin)

SOURCES += \
    FilterCopyMeshPlugin.cpp

HEADERS += \
    FilterCopyMeshPlugin.h
