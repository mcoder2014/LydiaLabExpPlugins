include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
StarlabTemplate(plugin)

SOURCES += main.cpp \
    FilterTransformationPlugin.cpp \
    TransformRichParameterWidget.cpp \
    TransformationTool.cpp

HEADERS += \
    FilterTransformationPlugin.h \
    TransformRichParameterWidget.h \
    TransformationTool.h

FORMS += \
    TransformRichParameterWidget.ui
