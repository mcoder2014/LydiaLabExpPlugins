include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
StarlabTemplate(plugin)

SOURCES += \
    FilterRevertFaceNormalPlugin.cpp

HEADERS += \
    FilterRevertFaceNormalPlugin.h
