#ifndef FILTERCOPYMESHPLUGIN_H
#define FILTERCOPYMESHPLUGIN_H

#include "SurfaceMeshPlugins.h"

class FilterCopyMeshPlugin
        :public SurfaceMeshFilterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "filter_copy_mesh.plugin.lydialab")
    Q_INTERFACES(FilterPlugin)

    // StarlabPlugin interface
public:
    virtual QString description() override{
        return tr("Copy a mesh");
    }
    virtual QString name() override {
        return tr("Shell Tool | Duplicate Mesh");
    }

    // FilterPlugin interface
public:
    virtual void applyFilter(RichParameterSet *richParameterSet) override;
    virtual void initParameters(RichParameterSet *richParameterSet) override;
    virtual bool isApplicable(Starlab::Model *model) override {
        return model != nullptr && isA(model);
    }
};

#endif // FILTERCOPYMESHPLUGIN_H
