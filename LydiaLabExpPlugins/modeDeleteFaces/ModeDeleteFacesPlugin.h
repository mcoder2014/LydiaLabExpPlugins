#ifndef MODEDELETEFACESPLUGIN_H
#define MODEDELETEFACESPLUGIN_H

#include "interfaces/ModePluginDockWidget.h"
#include "SurfaceMeshPlugins.h"

class ModeDeleteFacesPlugin
        :public SurfaceMeshModePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "lydiaLab_mode_delete_faces.plugin.lydialab")
    Q_INTERFACES(ModePlugin)
};

#endif // MODEDELETEFACESPLUGIN_H
