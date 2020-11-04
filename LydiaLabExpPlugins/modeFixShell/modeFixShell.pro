include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
StarlabTemplate(plugin)

SOURCES += \
    FixHoleTool.cpp \
    ModeFixPlugin.cpp \
    ModeFixWidget.cpp

HEADERS += \
    FixHoleTool.h \
    ModeFixPlugin.h \
    ModeFixWidget.h

FORMS += \
    ModeFixWidget.ui
