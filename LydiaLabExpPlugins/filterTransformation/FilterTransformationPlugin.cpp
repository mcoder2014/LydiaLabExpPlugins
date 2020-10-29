#include "FilterTransformationPlugin.h"

#include <iostream>

#include <Eigen/Dense>
#include <parameters/RichParameterWidget.h>

#include "TransformRichParameterWidget.h"
#include "TransformationTool.h"


ModeTransformationPlugin::ModeTransformationPlugin()
{
    transformationWidget = nullptr;
    controlDockWidget = nullptr;
}

/**
 * @brief FilterTransformationPlugin::create
 * 插件入口函数
 */
void ModeTransformationPlugin::create()
{
    // 再次进入插件
    if(transformationWidget || controlDockWidget) {
        controlDockWidget->setVisible(true);
        return;
    }

    // 初次初始化插件
    controlDockWidget = new ModePluginDockWidget(
                tr("Transformation"), mainWindow());
    transformationWidget = new TransformationWidget(controlDockWidget);
    controlDockWidget->setWidget(transformationWidget);
    controlDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    mainWindow()->addDockWidget(Qt::RightDockWidgetArea, controlDockWidget);

    // 建立事件处理函数链接
    initConnections();
    selectionChanged(safeCast(document()->selectedModel()));
}

/**
 * @brief ModeTransformationPlugin::destroy
 * 停止使用插件
 */
void ModeTransformationPlugin::destroy()
{
    if(controlDockWidget) {
        controlDockWidget->setVisible(false);
    }

    // 断开事件注册
    releaseConnections();
}

/**
 * @brief FilterTransformationPlugin::isApplicable
 * 是否可以应用该插件
 * @return
 */
bool ModeTransformationPlugin::isApplicable()
{
    return isA(this->document()->selectedModel());
}

/**
 * @brief ModeTransformationPlugin::suspend
 * 触发暂定操作
 */
void ModeTransformationPlugin::suspend(){releaseConnections();}

/**
 * @brief ModeTransformationPlugin::resume
 * 触发继续操作
 */
void ModeTransformationPlugin::resume(){
    initConnections();
    selectionChanged(safeCast(document()->selectedModel()));
}

/**
 * @brief ModeTransformationPlugin::initConnections
 */
void ModeTransformationPlugin::initConnections()
{
    // document selectionChanged 事件
    connect(document(), SIGNAL(selectionChanged(Model*)),
            this, SLOT(selectionChanged(Model*)));
    connect(transformationWidget, SIGNAL(transformationChagned(Vector3d, Vector3d, Vector3d, Vector3d)),
            this, SLOT(applyTransform(Vector3d, Vector3d, Vector3d, Vector3d)),
            Qt::UniqueConnection);
}

/**
 * @brief ModeTransformationPlugin::releaseConnections
 */
void ModeTransformationPlugin::releaseConnections()
{
    disconnect(document(), SIGNAL(selectionChanged(Model*)),
            this, SLOT(selectionChanged(Model*)));
    disconnect(transformationWidget, SIGNAL(transformationChagned(Vector3d, Vector3d, Vector3d, Vector3d)),
            this, SLOT(applyTransform(Vector3d, Vector3d, Vector3d, Vector3d)));
}

/**
 * @brief ModeTransformationPlugin::selectionChanged
 * @param model
 */
void ModeTransformationPlugin::selectionChanged(Model *model)
{
    this->transformationWidget->update(model);
}

void ModeTransformationPlugin::applyTransform(
        Eigen::Vector3d position,
        Eigen::Vector3d rotation,
        Eigen::Vector3d scale,
        Eigen::Vector3d size)
{
    SurfaceMeshModel* model = safeCast(document()->selectedModel());
    if(model == nullptr){
        std::cout << __FILE__ << __LINE__ << __PRETTY_FUNCTION__
                  << "model == nullptr" << std::endl;
        return;
    }
    Eigen::Matrix4d oldTransform = model->getTransformationMatrix();
    model->setPosition(position);
    model->setRotation(rotation);

    // 计算 Scale
    Vector3d oldScale = model->getScale();
    Vector3d oldSize = model->bbox().max()-model->bbox().min();
    scale.x() = size.x()/oldSize.x() * oldScale.x();
    scale.y() = size.y()/oldSize.y() * oldScale.y();
    scale.z() = size.z()/oldSize.z() * oldScale.z();
    model->setScale(scale);

    Eigen::Matrix4d transformMatrix = model->getTransformationMatrix();
    transformMatrix = transformMatrix * oldTransform.inverse();

    // 执行变换
    transformation(model, transformMatrix);

    // 手动触发更新
    model->updateBoundingBox();
    drawArea()->update();
    selectionChanged(model);
}
