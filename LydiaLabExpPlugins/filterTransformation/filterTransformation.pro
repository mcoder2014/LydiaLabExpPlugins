include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
StarlabTemplate(plugin)

SOURCES += main.cpp \
    FilterTransformationPlugin.cpp \
    TransformRichParameterWidget.cpp

HEADERS += \
    FilterTransformationPlugin.h \
    TransformRichParameterWidget.h

FORMS += \
    TransformRichParameterWidget.ui
