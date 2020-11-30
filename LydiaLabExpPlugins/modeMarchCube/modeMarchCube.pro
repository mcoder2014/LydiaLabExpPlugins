include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
StarlabTemplate(plugin)

# 基于 March Cube 算法的插件

HEADERS += \
    ModeMarchCubePlugin.h

SOURCES += \
    ModeMarchCubePlugin.cpp
