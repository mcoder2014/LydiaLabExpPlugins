#ifndef FILTERTRANSFORMATIONPLUGIN_H
#define FILTERTRANSFORMATIONPLUGIN_H


#include "interfaces/ModePluginDockWidget.h"
#include "SurfaceMeshPlugins.h"

class TransformationWidget;

/**
 * @brief The ModeTransformationPlugin class
 * 调整模型位置的插件
 *
 */
class ModeTransformationPlugin
        :public SurfaceMeshModePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "lydiaLab_mode_transformation.plugin.lydialab")
    Q_INTERFACES(ModePlugin)

public:
    ModeTransformationPlugin();
    virtual ~ModeTransformationPlugin() override{}

    virtual QString name() override { return "Transformation"; }
    virtual QIcon icon() override {return QIcon(":/resources/Axis.png");}
    virtual QString description() override { return "Transformation"; }

public:

    virtual void create() override;
    virtual void destroy() override;
    virtual bool documentChanged() override { return true; }

    // 检查插件启动条件
    virtual bool isApplicable() override;

    virtual void suspend() override;
    virtual void resume() override;

    virtual QString filter() override {
        return QString("model (*.stl *.obj *.ply)"); }

private:
    TransformationWidget* transformationWidget;
    ModePluginDockWidget* controlDockWidget;

    // 建立链接
    void initConnections();

    // 断开链接
    void releaseConnections();

private slots:
    // 处理 Document 中当前选中的模型发生改变事件
    void selectionChanged(Model* model);
    void applyTransform(Vector3d position, Vector3d rotation,
                        Vector3d scale, Vector3d size);
};

#endif // FILTERTRANSFORMATIONPLUGIN_H
