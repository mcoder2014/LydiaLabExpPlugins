include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
StarlabTemplate(plugin)

SOURCES += \
    ModeTransformationPlugin.cpp \
    TransformationTool.cpp \
    TransformationWidget.cpp

HEADERS += \
    ModeTransformationPlugin.h \
    TransformationTool.h \
    TransformationWidget.h

FORMS += \
    TransformationWidget.ui

RESOURCES += \
    transformationpluginresource.qrc
