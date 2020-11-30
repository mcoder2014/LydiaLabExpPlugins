#ifndef MODEPARTIALMORPHPLUGIN_H
#define MODEPARTIALMORPHPLUGIN_H

#include "interfaces/ModePluginDockWidget.h"
#include "SurfaceMeshPlugins.h"

class ModePartialMorphPlugin
        :public SurfaceMeshModePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "lydiaLab_mode_partial_morph.plugin.lydialab")
    Q_INTERFACES(ModePlugin)

public:
    ModePartialMorphPlugin();
};

#endif // MODEPARTIALMORPHPLUGIN_H
