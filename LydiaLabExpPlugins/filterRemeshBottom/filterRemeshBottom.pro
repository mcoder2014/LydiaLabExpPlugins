include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
include($$PWD/../depends/octree_starlab.pri)
StarlabTemplate(plugin)

HEADERS += \
    Delaunator.h \
    FilterRemeshBottomPlugin.h \
    RemeshBottomTool.h

SOURCES += \
    Delaunator.cpp \
    FilterRemeshBottomPlugin.cpp \
    RemeshBottomTool.cpp
