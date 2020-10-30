include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
include($$PWD/../depends/octree_starlab.pri)
StarlabTemplate(plugin)

HEADERS += \
    FilterRemeshBottomPlugin.h \
    RemeshBottomTool.h

SOURCES += \
    FilterRemeshBottomPlugin.cpp \
    RemeshBottomTool.cpp
