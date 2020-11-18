#ifndef FILTERMINIMALSURFACEPLUGIN_H
#define FILTERMINIMALSURFACEPLUGIN_H

#include "SurfaceMeshPlugins.h"

class FilterMinimalSurfacePlugin
        : public SurfaceMeshFilterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "filter_minimal_surface.plugin.lydialab")
    Q_INTERFACES(FilterPlugin)

    void applyFilter(RichParameterSet*) override;
    void initParameters(RichParameterSet*) override{}

    // StarlabPlugin interface
public:
    QString name() override {
        return tr("Smoothing | Minimal Surface");
    }

    QString description() override {
        return tr("Minimal Surface generate");
    }

    // FilterPlugin interface
public:
    bool isApplicable(Starlab::Model *model) override;

};

#endif // FILTERMINIMALSURFACEPLUGIN_H
