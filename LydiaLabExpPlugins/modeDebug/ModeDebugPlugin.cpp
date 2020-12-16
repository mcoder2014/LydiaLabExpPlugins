#include "ModeDebugPlugin.h"

#include "ui_ModeDebugPluginWidget.h"
#include "algorithm/RemoveUnconnectedVertex.h"

ModeDebugPlugin::ModeDebugPlugin()
{
    modeDebugPluginWidget = nullptr;
    controlDockWidget = nullptr;
}

void ModeDebugPlugin::create()
{
    if( !modeDebugPluginWidget && !controlDockWidget) {
        controlDockWidget = new ModePluginDockWidget(tr("Debug Tool"), mainWindow());
        modeDebugPluginWidget = new ModeDebugPluginWidget;
        controlDockWidget->setWidget(modeDebugPluginWidget);
        controlDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
        mainWindow()->addDockWidget(Qt::RightDockWidgetArea, controlDockWidget);
    }

    controlDockWidget->setVisible(true);
    initConnections();
}

void ModeDebugPlugin::destroy()
{
    if(controlDockWidget) {
        controlDockWidget->setVisible(false);
    }
    releaseConnections();
}

void ModeDebugPlugin::suspend()
{
    releaseConnections();
}

void ModeDebugPlugin::resume()
{
    initConnections();
}

void ModeDebugPlugin::initConnections()
{
    Ui::ModeDebugPluginWidget* widget = modeDebugPluginWidget->getUi();
    connect(widget->pushButtonUpdateVertexNormal, SIGNAL(clicked(bool)),
            this, SLOT(updateVertexNormals()), Qt::UniqueConnection);
    connect(widget->pushButtonUpdateFaceNormal, SIGNAL(clicked(bool)),
            this, SLOT(updateFaceNormals()), Qt::UniqueConnection);
    connect(widget->pushButtonUpdateBoundingBox, SIGNAL(clicked(bool)),
            this, SLOT(updateBoundingBox()), Qt::UniqueConnection);
    connect(widget->pushButtonRemoveUnconnectedFaces, SIGNAL(clicked(bool)),
            this, SLOT(removeUnconnectedVertex()), Qt::UniqueConnection);
}

void ModeDebugPlugin::releaseConnections()
{
    Ui::ModeDebugPluginWidget* widget = modeDebugPluginWidget->getUi();
    disconnect(widget->pushButtonUpdateVertexNormal, SIGNAL(clicked(bool)),
            this, SLOT(updateVertexNormals()));
    disconnect(widget->pushButtonUpdateFaceNormal, SIGNAL(clicked(bool)),
            this, SLOT(updateFaceNormals()));
    disconnect(widget->pushButtonUpdateBoundingBox, SIGNAL(clicked(bool)),
            this, SLOT(updateBoundingBox()));
    disconnect(widget->pushButtonRemoveUnconnectedFaces, SIGNAL(clicked(bool)),
            this, SLOT(removeUnconnectedVertex()));
}

void ModeDebugPlugin::updateVertexNormals()
{
    SurfaceMeshModel* model = safeCast(document()->selectedModel());
    model->update_vertex_normals();
    mainWindow()->setStatusBarMessage(QString("%1_finished").arg(__PRETTY_FUNCTION__));
}

void ModeDebugPlugin::updateFaceNormals()
{
    SurfaceMeshModel* model = safeCast(document()->selectedModel());
    model->update_face_normals();
    mainWindow()->setStatusBarMessage(QString("%1_finished").arg(__PRETTY_FUNCTION__));
}

void ModeDebugPlugin::updateBoundingBox()
{
    SurfaceMeshModel* model = safeCast(document()->selectedModel());
    model->updateBoundingBox();
    mainWindow()->setStatusBarMessage(QString("%1_finished").arg(__PRETTY_FUNCTION__));

}

/**
 * @brief ModeDebugPlugin::removeUnconnectedVertex
 * 移除离散顶点
 */
void ModeDebugPlugin::removeUnconnectedVertex()
{
    SurfaceMeshModel* model = mesh();
    SurfaceMeshModel* target = removeUnconnectedFace(model);
    document()->addModel(target);
    mainWindow()->setStatusBarMessage(QString("%1_finished").arg(__PRETTY_FUNCTION__));
}
