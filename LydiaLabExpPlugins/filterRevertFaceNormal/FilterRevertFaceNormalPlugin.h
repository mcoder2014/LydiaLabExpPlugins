#ifndef FILTERREVERTFACENORMALPLUGIN_H
#define FILTERREVERTFACENORMALPLUGIN_H

#include <SurfaceMeshPlugins.h>

class FilterRevertFaceNormalPlugin
        :public SurfaceMeshFilterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "filter_revert_face_normal.plugin.lydialab")
    Q_INTERFACES(FilterPlugin)


    // StarlabPlugin interface
public:
    virtual QString description() override {
        return tr("Revert surfaceMeshModel through revert revert the vertex order in one face.");
    }
    virtual QString name() override {
        return tr("RevertFaceNormal");
    }

    // FilterPlugin interface
public:
    virtual void applyFilter(RichParameterSet *) override;
    virtual bool isApplicable(Starlab::Model *model) override {
        return model != nullptr && isA(model);
    }
};

#endif // FILTERREVERTFACENORMALPLUGIN_H
