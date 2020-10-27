include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
include($$PWD/../depends/openmesh.pri)
StarlabTemplate(plugin)

HEADERS += \
    Convertor.h \
    IOOpenmeshPlugin.h \
    Mesh.h

SOURCES += \
    Convertor.cpp \
    IOOpenmeshPlugin.cpp
