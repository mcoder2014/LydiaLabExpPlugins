#ifndef FILTERTRANSFORMATIONPLUGIN_H
#define FILTERTRANSFORMATIONPLUGIN_H

#include "SurfaceMeshPlugins.h"

class FilterTransformationPlugin
        :public SurfaceMeshFilterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "lydiaLab_filter_simplification.plugin.lydialab")
    Q_INTERFACES(FilterPlugin)

public:
    virtual QString name() override { return "Transformation | Position"; }
    virtual QString description() override { return "Transformation"; }

    // 初始化参数窗口
    virtual void initParameters(RichParameterSet *richParameterSet) override;

    // 应用
    virtual void applyFilter(RichParameterSet* richParameterSet) override;

    // FilterPlugin interface
public:
    virtual bool isApplicable(Starlab::Model *model) override;
};

#endif // FILTERTRANSFORMATIONPLUGIN_H
