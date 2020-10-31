#ifndef MODEDEBUGPLUGIN_H
#define MODEDEBUGPLUGIN_H

#include "SurfaceMeshPlugins.h"
#include "SurfaceMeshModel.h"
#include "interfaces/ModePluginDockWidget.h"

#include "ModeDebugPluginWidget.h"

class ModeDebugPlugin
        :public SurfaceMeshModePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "mode_debug.plugin.lydialab")
    Q_INTERFACES(ModePlugin)

public:
    ModeDebugPlugin();
    ~ModeDebugPlugin() override {

    }

    // StarlabPlugin interface
public:
    virtual QString description() override {
        return tr("Expose part of the function interface for debug");}
    virtual QString name() override {return tr("Debug");}
    virtual QIcon icon() override{return QIcon("://resources/bug.png");}

    // ModePlugin interface
public:
    virtual bool isApplicable() override {
        return isA(document()->selectedModel());
    }
    virtual void create() override;
    virtual void destroy() override;
    virtual void suspend() override;
    virtual void resume() override;

private:
    ModeDebugPluginWidget* modeDebugPluginWidget;
    ModePluginDockWidget* controlDockWidget;

    void initConnections();
    void releaseConnections();

public slots:
    void updateVertexNormals();
    void updateFaceNormals();
    void updateBoundingBox();
};

#endif // MODEDEBUGPLUGIN_H
