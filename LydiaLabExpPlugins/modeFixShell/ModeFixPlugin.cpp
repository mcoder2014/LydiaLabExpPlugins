#include "ModeFixPlugin.h"

#include "FixHoleTool.h"
#include "ui_ModeFixWidget.h"

ModeFixPlugin::ModeFixPlugin()
{
    modeFixWidget = nullptr;
    modePluginDockWidget = nullptr;

    topSurface = nullptr;
    bottomSurface = nullptr;
}

void ModeFixPlugin::create()
{
    if(!modeFixWidget && !modePluginDockWidget) {
        modePluginDockWidget = new ModePluginDockWidget(tr("Mode Fix"), mainWindow());
        modeFixWidget = new ModeFixWidget(modePluginDockWidget);
        initConnections();

        modePluginDockWidget->setWidget(modeFixWidget);
        modePluginDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
        mainWindow()->addDockWidget(Qt::RightDockWidgetArea, modePluginDockWidget);
    }

    modePluginDockWidget->setVisible(true);
}

void ModeFixPlugin::destroy()
{
    modePluginDockWidget->setVisible(false);
}

void ModeFixPlugin::suspend()
{
    modePluginDockWidget->setVisible(false);
}

void ModeFixPlugin::resume()
{
    modePluginDockWidget->setVisible(true);
}

/**
 * @brief ModeFixPlugin::apply
 * 连接网格的上表面和下表面，涉及到网格的坐标系转换
 * 最终以下表面的坐标系为准
 * 1. 上表面转换到世界坐标系
 * 2. 上表面转换到下表面坐标系
 * 3. 生成的新网格模型按照下表面的坐标系
 */
void ModeFixPlugin::apply()
{
    /// 将上表面转移到下表面坐标系
    SurfaceMeshModel* topSurfaceClone = new SurfaceMeshModel;
    topSurfaceClone->assign(*topSurface);

    Matrix4d bottomTransMat = bottomSurface->getTransformationMatrix();
    Matrix4d topTransMat = topSurfaceClone->getTransformationMatrix();
    Matrix4d transMat = (bottomTransMat.inverse()) * topTransMat;
    transformation(topSurfaceClone, transMat);

    SurfaceMeshModel* model = fixHole(
                topSurfaceClone, bottomSurface,
                modeFixWidget->ui->spinBoxPointsNumber->value());

    // 释放 topSurfaceClone
    delete topSurfaceClone;

    model->updateBoundingBox();
    model->update_face_normals();
    model->update_vertex_normals();

    // 修改名称
    model->name = modeFixWidget->ui->lineEditTargetName->text();

    document()->addModel(model);

    mainWindow()->setStatusBarMessage("Apply successed");
}

void ModeFixPlugin::initConnections()
{
    connect(modeFixWidget->ui->pushButtonTop, &QPushButton::clicked,
            this, [this](){this->setSurfaceMesh(topSurface);}, Qt::UniqueConnection);
    connect(modeFixWidget->ui->pushButtonBottom, &QPushButton::clicked,
            this, [this](){this->setSurfaceMesh(bottomSurface);},Qt::UniqueConnection);
    connect(modeFixWidget->ui->pushButtonApply, SIGNAL(clicked(bool)),
            this, SLOT(apply()),Qt::UniqueConnection);
}

void ModeFixPlugin::setSurfaceMesh(SurfaceMeshModel* &model)
{
    model = safeCast(document()->selectedModel());
    mainWindow()->setStatusBarMessage("Set model " + model->name);
}
