#include "ModeTransformationPlugin.h"

#include <iostream>

#include <Eigen/Dense>
#include <parameters/RichParameterWidget.h>

#include "TransformationWidget.h"
#include "TransformationTool.h"

using Eigen::Vector4d;

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
    if(!transformationWidget && !controlDockWidget) {
        // 初次初始化插件
        controlDockWidget = new ModePluginDockWidget(tr("Transformation"), mainWindow());
        transformationWidget = new TransformationWidget(controlDockWidget);
        controlDockWidget->setWidget(transformationWidget);
        controlDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
        mainWindow()->addDockWidget(Qt::RightDockWidgetArea, controlDockWidget);
    }

    // 显示控制界面
    controlDockWidget->setVisible(true);
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

    // 应用位移操作
    connect(transformationWidget, SIGNAL(transformationChagned(Vector3d, Vector3d, Vector3d, Vector3d)),
            this, SLOT(applyTransform(Vector3d, Vector3d, Vector3d, Vector3d)),
            Qt::UniqueConnection);

    // 更新坐标原点到模型的中心
    connect(transformationWidget->pushButtonSetModelCenterAsOrigin, &QPushButton::clicked,
            [&](){
        // 减少不必要的调用
        Vector3d localCenter3d = mesh()->bbox().center();
        if(equal(localCenter3d, Vector3d::Zero())){
            std::cout << __FUNCTION__ << __LINE__ << std::endl;
                return;
        }

        // 获得 世界坐标 和 局部坐标 之间的转换矩阵
        Matrix4d transMat = mesh()->getTransformationMatrix();

        // Center 的两种坐标
        Vector4d localCenter4d;
        localCenter4d << localCenter3d, 1;
        Vector4d worldCenter4d = transMat * localCenter4d;

        // Position 的两种坐标
        Vector4d worldPosition4d;
        worldPosition4d << mesh()->getPosition(), 1;

        Vector4d localOrigin4d(0, 0, 0, 1);

        // 模型顶点的坐标变换
        setModelOrigin(mesh(), localCenter4d.segment(0, 3));
        mesh()->setPosition(worldCenter4d.segment(0, 3));
        transformationWidget->updateUI(mesh()->getPosition(), mesh()->getRotation(), mesh()->getScale(), mesh()->bbox());
        transformationWidget->pushButtonSetModelCenterAsOrigin->setEnabled(false);
        transformationWidget->pushButtonSetAxisOriginAsOrigin->setEnabled(true);
    });


    // 更新坐标原点到坐标系原点
    connect(transformationWidget->pushButtonSetAxisOriginAsOrigin, &QPushButton::clicked,
            [&](){
        // 获得 世界坐标 和 局部坐标 之间的转换矩阵
        Matrix4d transMat = mesh()->getTransformationMatrix();
        Matrix4d invTransMat = transMat.inverse();

        // old model local origin
        Vector4d localOldOrigin4d(0, 0, 0, 1);

        // Axis Origin
        Vector4d worldOrigin4d(0.0, 0.0, 0.0, 1.0);
        Vector4d localOrigin4d = invTransMat * worldOrigin4d;

        // 修改模型顶点数据
        setModelOrigin(mesh(), (localOrigin4d - localOldOrigin4d).segment(0, 3));
        mesh()->setPosition((worldOrigin4d).segment(0, 3));
        transformationWidget->updateUI(mesh()->getPosition(), mesh()->getRotation(), mesh()->getScale(), mesh()->bbox());
        transformationWidget->pushButtonSetModelCenterAsOrigin->setEnabled(true);
        transformationWidget->pushButtonSetAxisOriginAsOrigin->setEnabled(false);
    });

    connectTrigger();
}

void ModeTransformationPlugin::connectTrigger()
{
    /// Position
    connect(transformationWidget->doubleSpinBoxPosX, SIGNAL(valueChanged(double)),
            this, SLOT(apply()), Qt::UniqueConnection);
    connect(transformationWidget->doubleSpinBoxPosY, SIGNAL(valueChanged(double)),
            this, SLOT(apply()), Qt::UniqueConnection);
    connect(transformationWidget->doubleSpinBoxPosZ, SIGNAL(valueChanged(double)),
            this, SLOT(apply()), Qt::UniqueConnection);

    /// Rotation
    connect(transformationWidget->doubleSpinBoxRotX, SIGNAL(valueChanged(double)),
            this, SLOT(apply()), Qt::UniqueConnection);
    connect(transformationWidget->doubleSpinBoxRotY, SIGNAL(valueChanged(double)),
            this, SLOT(apply()), Qt::UniqueConnection);
    connect(transformationWidget->doubleSpinBoxRotZ, SIGNAL(valueChanged(double)),
            this, SLOT(apply()), Qt::UniqueConnection);

    /// Scale
    connect(transformationWidget->doubleSpinBoxBboxX, SIGNAL(valueChanged(double)),
            this, SLOT(apply()), Qt::UniqueConnection);
    connect(transformationWidget->doubleSpinBoxBboxY, SIGNAL(valueChanged(double)),
            this, SLOT(apply()), Qt::UniqueConnection);
    connect(transformationWidget->doubleSpinBoxBboxZ, SIGNAL(valueChanged(double)),
            this, SLOT(apply()), Qt::UniqueConnection);
}

void ModeTransformationPlugin::disconnectTrigger()
{
    /// Position
    disconnect(transformationWidget->doubleSpinBoxPosX, SIGNAL(valueChanged(double)),
            this, SLOT(apply()));
    disconnect(transformationWidget->doubleSpinBoxPosY, SIGNAL(valueChanged(double)),
            this, SLOT(apply()));
    disconnect(transformationWidget->doubleSpinBoxPosZ, SIGNAL(valueChanged(double)),
            this, SLOT(apply()));

    /// Rotation
    disconnect(transformationWidget->doubleSpinBoxRotX, SIGNAL(valueChanged(double)),
            this, SLOT(apply()));
    disconnect(transformationWidget->doubleSpinBoxRotY, SIGNAL(valueChanged(double)),
            this, SLOT(apply()));
    disconnect(transformationWidget->doubleSpinBoxRotZ, SIGNAL(valueChanged(double)),
            this, SLOT(apply()));

    /// Scale
    disconnect(transformationWidget->doubleSpinBoxBboxX, SIGNAL(valueChanged(double)),
            this, SLOT(apply()));
    disconnect(transformationWidget->doubleSpinBoxBboxY, SIGNAL(valueChanged(double)),
            this, SLOT(apply()));
    disconnect(transformationWidget->doubleSpinBoxBboxZ, SIGNAL(valueChanged(double)),
            this, SLOT(apply()));
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
    disconnectTrigger();

    transformationWidget->update(model);

    connectTrigger();
}

void ModeTransformationPlugin::apply()
{
    Vector3d position;
    Vector3d rotation;
    Vector3d size;

    position.x() = transformationWidget->doubleSpinBoxPosX->value();
    position.y() = transformationWidget->doubleSpinBoxPosY->value();
    position.z() = transformationWidget->doubleSpinBoxPosZ->value();

    rotation.x() = transformationWidget->ang2rad(transformationWidget->doubleSpinBoxRotX->value());
    rotation.y() = transformationWidget->ang2rad(transformationWidget->doubleSpinBoxRotY->value());
    rotation.z() = transformationWidget->ang2rad(transformationWidget->doubleSpinBoxRotZ->value());

    size.x() = transformationWidget->doubleSpinBoxBboxX->value();
    size.y() = transformationWidget->doubleSpinBoxBboxY->value();
    size.z() = transformationWidget->doubleSpinBoxBboxZ->value();

    applyTransform(position, rotation, size);
}

void ModeTransformationPlugin::applyTransform(
        Eigen::Vector3d position,
        Eigen::Vector3d rotation,
        Eigen::Vector3d size)
{
    SurfaceMeshModel* model = mesh();
    if(model == nullptr){
        std::cout << __FILE__ << __LINE__ << __PRETTY_FUNCTION__
                  << "model == nullptr" << std::endl;
        return;
    }

    // 计算 Scale
    Vector3d scale;
    Vector3d bboxSize = model->bbox().max()-model->bbox().min();
    scale.x() = size.x()/bboxSize.x();
    scale.y() = size.y()/bboxSize.y();
    scale.z() = size.z()/bboxSize.z();

    // 更新模型的数据
    model->setPosition(position);
    model->setRotation(rotation);
    model->setScale(scale);

    // 手动触发更新
    drawArea()->updateGL();
    selectionChanged(model);
}
