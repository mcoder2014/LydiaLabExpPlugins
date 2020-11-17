#ifndef FILTERMINIMALSURFACEPLUGIN_H
#define FILTERMINIMALSURFACEPLUGIN_H

#include "SurfaceMeshPlugins.h"

class FilterMinimalSurfacePlugin
        : public SurfaceMeshFilterPlugin
{

    void applyFilter(RichParameterSet*) override;
    void initParameters(RichParameterSet*) override;

    // StarlabPlugin interface
public:
    QString name() override {
        return tr("Smoothing | Minimal Surface");
    };
};

#endif // FILTERMINIMALSURFACEPLUGIN_H
