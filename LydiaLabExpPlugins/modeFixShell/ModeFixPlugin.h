#ifndef MODEFIXPLUGIN_H
#define MODEFIXPLUGIN_H

#include <SurfaceMeshPlugins.h>
#include <interfaces/ModePluginDockWidget.h>

#include "ModeFixWidget.h"

class ModeFixPlugin
        :public SurfaceMeshModePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "lydiaLab_mode_fix_shell_hole.plugin.lydialab")
    Q_INTERFACES(ModePlugin)

public:
    ModeFixPlugin();
    virtual ~ModeFixPlugin() override {}

private:
    ModePluginDockWidget* modePluginDockWidget;
    ModeFixWidget* modeFixWidget;

    // 上表面
    SurfaceMeshModel* topSurface;

    // 下表面
    SurfaceMeshModel* bottomSurface;

    // StarlabPlugin interface
public:
    virtual QString description() override{
        return tr("Repair the hole between the two surfaces");
    }

    virtual QString name() override{
        return tr("Connect Top and Bottom");
    }

    // ModePlugin interface
public:
    virtual bool isApplicable() override {
        return isA(document()->selectedModel());
    }

    virtual void create() override;
    virtual void destroy() override;
    virtual void suspend() override;
    virtual void resume() override;

public slots:
    void apply();
    void initConnections();
    void releaseConnections();
    void setSurfaceMesh(SurfaceMeshModel* &model);
};

#endif // MODEFIXPLUGIN_H
