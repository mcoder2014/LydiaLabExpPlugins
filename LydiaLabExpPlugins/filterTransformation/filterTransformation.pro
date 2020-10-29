include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
StarlabTemplate(plugin)

SOURCES += \
    FilterTransformationPlugin.cpp \
    TransformRichParameterWidget.cpp \
    TransformationTool.cpp

HEADERS += \
    FilterTransformationPlugin.h \
    TransformRichParameterWidget.h \
    TransformationTool.h

FORMS += \
    TransformRichParameterWidget.ui

RESOURCES += \
    transformationpluginresource.qrc
