#ifndef FILTERMIRRORPLUGIN_H
#define FILTERMIRRORPLUGIN_H

#include "SurfaceMeshPlugins.h"

class FilterMirrorPlugin
        :public SurfaceMeshFilterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "filter_mirror.plugin.lydialab")
    Q_INTERFACES(FilterPlugin)

public:
    FilterMirrorPlugin();

    // StarlabPlugin interface
public:
    virtual QString description() override{
        return "Miirror the mesh through a plane. "; }
    virtual QString name() override{
        return "Transformation | mirror";}

    // FilterPlugin interface
public:
    virtual void applyFilter(RichParameterSet *richParameterSet) override;
    virtual void initParameters(RichParameterSet *richParameterSet) override;
    virtual bool isApplicable(Starlab::Model *model) override;
};

#endif // FILTERMIRRORPLUGIN_H
