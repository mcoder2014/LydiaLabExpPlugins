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
    QString name() { return "Transformation | Position"; }
    QString description() { return "Transformation"; }

    // 初始化参数窗口
    void initParameters(RichParameterSet *richParameterSet);

    // 应用
    void applyFilter(RichParameterSet* richParameterSet);
};

#endif // FILTERTRANSFORMATIONPLUGIN_H
