#ifndef MODEMARCHCUBEPLUGIN_H
#define MODEMARCHCUBEPLUGIN_H

#include "interfaces/ModePluginDockWidget.h"
#include "SurfaceMeshPlugins.h"

class ModeMarchCubePlugin
        :public SurfaceMeshModePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "lydiaLab_mode_march_cube.plugin.lydialab")
    Q_INTERFACES(ModePlugin)

};

#endif // MODEMARCHCUBEPLUGIN_H
