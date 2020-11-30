include($$[STARLAB])
include($$[SURFACEMESH])
include($$PWD/../depends/eigen.pri)
include($$PWD/../depends/cholmod.pri)

StarlabTemplate(plugin)

HEADERS += \
    surfacemesh_mode_arapdeform.h \
    ARAPDeformer.h \
    ARAPDeformerHandle.h \
    arap_globals.h \
    arap_widget.h
SOURCES += \
    surfacemesh_mode_arapdeform.cpp \
    ARAPDeformer.cpp \
    arap_widget.cpp

RESOURCES += surfacemesh_mode_arapdeform.qrc

FORMS += \
    arap_widget.ui
