include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
include($$PWD/../depends/octree_starlab.pri)
StarlabTemplate(plugin)

SOURCES += \
    ModeDebugPlugin.cpp \
    ModeDebugPluginWidget.cpp

HEADERS += \
    ModeDebugPlugin.h \
    ModeDebugPluginWidget.h

FORMS += \
    ModeDebugPluginWidget.ui

RESOURCES += \
    modedebugresource.qrc
