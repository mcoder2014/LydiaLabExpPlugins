#ifndef FILTERREMESHBOTTOMPLUGIN_H
#define FILTERREMESHBOTTOMPLUGIN_H

#include <SurfaceMeshPlugins.h>

class FilterRemeshBottomPlugin
        :public SurfaceMeshFilterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "filter_remesh_bottom.plugin.lydialab")
    Q_INTERFACES(FilterPlugin)

    // StarlabPlugin interface
public:
    virtual QString description() override {
        return tr("Reconstruct model bottom using ray and sampling points");
    }
    virtual QString name() override {
        return tr("Remesh | RemeshBottom");
    }

    // FilterPlugin interface
public:
    virtual void applyFilter(RichParameterSet *richParameterSet) override;

    virtual void initParameters(RichParameterSet *richParameterSet) override;

    virtual bool isApplicable(Starlab::Model *model) override {
        return model != nullptr && isA(model);
    }
};

#endif // FILTERREMESHBOTTOMPLUGIN_H
