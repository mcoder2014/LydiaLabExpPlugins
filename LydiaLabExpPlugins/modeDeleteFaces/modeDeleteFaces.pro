include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
StarlabTemplate(plugin)

# 因为模型边界处不均匀，删除边缘部分面片所用

HEADERS += \
    DeleteFacesWidget.h \
    ModeDeleteFacesPlugin.h

SOURCES += \
    DeleteFacesWidget.cpp \
    ModeDeleteFacesPlugin.cpp

FORMS += \
    DeleteFacesWidget.ui

RESOURCES += \
    sources.qrc
