include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
include($$PWD/../depends/assimp.pri)
StarlabTemplate(plugin)

HEADERS += \
    IOAssimp.h

SOURCES += \
    IOAssimp.cpp
