#ifndef FILTEROFFSETPLUGIN_H
#define FILTEROFFSETPLUGIN_H

#include <SurfaceMeshPlugins.h>

/**
 * @brief The FilterOffsetPlugin class
 * 将网格复制两份，
 * 一份正向移动、一份负向移动
 */
class FilterOffsetPlugin
        :public SurfaceMeshFilterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "filter_offset_surface.plugin.lydialab")
    Q_INTERFACES(FilterPlugin)
public:
    virtual QString description() override {
        return tr("Duplicate the surface and move in the opposite direction of the normal");
    }

    virtual QString name() override {
        return tr("Shell Tool | Offset");
    }

    // 应用Filter
    virtual void applyFilter(RichParameterSet *richParameterSet) override;

    // 初始化插件
    virtual void initParameters(RichParameterSet *richParameterSet) override;

    virtual bool isApplicable(Starlab::Model *model) override {
        return model != nullptr && isA(model);
    }

};

#endif // FILTEROFFSETPLUGIN_H
