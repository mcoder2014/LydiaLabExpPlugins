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

void ModeFixPlugin::apply()
{
    SurfaceMeshModel* model = fixHole(
                topSurface, bottomSurface,
                modeFixWidget->ui->spinBoxPointsNumber->value());

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
