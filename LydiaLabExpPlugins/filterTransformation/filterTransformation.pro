include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
StarlabTemplate(plugin)

SOURCES += main.cpp \
    FilterTransformationPlugin.cpp

HEADERS += \
    FilterTransformationPlugin.h
