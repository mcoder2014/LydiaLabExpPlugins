include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
StarlabTemplate(plugin)

# 因为模型边界处不均匀，删除边缘部分面片所用

HEADERS += \
    ModeDeleteFacesPlugin.h

SOURCES += \
    ModeDeleteFacesPlugin.cpp
