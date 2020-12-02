#include "ModeDeleteFacesPlugin.h"

#include "ui_DeleteFacesWidget.h"

/**
 * @brief ModeDeleteFacesPlugin::ModeDeleteFacesPlugin
 * 默认构造函数
 */
ModeDeleteFacesPlugin::ModeDeleteFacesPlugin()
    :SurfaceMeshModePlugin()
{
    this->modePluginDockWidegt = nullptr;
    this->deleteFacesWidget = nullptr;
    this->currentMode = SELECT;

    paint_cursor = QCursor(QPixmap(":/sources/paint.png"), 0, 32);
    erase_cursor = QCursor(QPixmap(":/sources/erase.png"), 0, 32);
    deform_cursor = QCursor(QPixmap(":/sources/deform_cursor.png"), 0, 0);

    brushSize = 10;
    isDrawBrushSize = false;
}

/**
 * @brief ModeDeleteFacesPlugin::create
 * 创建插件
 */
void ModeDeleteFacesPlugin::create()
{
    if(!modePluginDockWidegt && !deleteFacesWidget) {
        // 初次初始化插件
        modePluginDockWidegt = new ModePluginDockWidget(tr("Delete Faces"), mainWindow());
        deleteFacesWidget = new DeleteFacesWidget(modePluginDockWidegt);
        modePluginDockWidegt->setWidget(deleteFacesWidget);
        modePluginDockWidegt->setFeatures(QDockWidget::NoDockWidgetFeatures);
        mainWindow()->addDockWidget(Qt::RightDockWidgetArea, modePluginDockWidegt);

        initConnect();
    }

    modePluginDockWidegt->setVisible(true);
    selectionChanged();
}

void ModeDeleteFacesPlugin::destroy()
{
    modePluginDockWidegt->setVisible(false);
}

/**
 * @brief ModeDeleteFacesPlugin::selectionChanged
 * 当场景中更新后，尝试更新数据
 * @return
 */
bool ModeDeleteFacesPlugin::selectionChanged()
{
    std::cout << __PRETTY_FUNCTION__ << __LINE__ << std::endl;

    if(document()->selectedModel() == nullptr) {
        return false;
    }

    // clean selected faces
    this->selectedFaces.clear();
    deleteFacesWidget->ui->faceCounts->setValue(0);

    points = mesh()->vertex_property<Vector3>(VPOINT);
    fnormals = mesh()->face_property<Vector3>(FNORMAL);

    return true;
}

/**
 * @brief ModeDeleteFacesPlugin::deleteFace
 * 执行删除面片操作
 */
void ModeDeleteFacesPlugin::deleteFace()
{
    SurfaceMeshModel *model = mesh();

    /// 删除选中的面片，直接在模型上修改
//    for(Face face : selectedFaces) {
//        std::cout << "remove face " << face.idx() << std::endl;
//        model->remove_face(face);
//    }

//    // 执行垃圾回收算法
//    model->garbage_collection();

//    // 重新加载 renderer
//    model->renderer()->init();

    /// tmp 先使用粗糙手段，重新构建一个SurfaceMesh。
    vector<Face> faceSubSet;
    for(Face face : model->faces()) {
        if(selectedFaces.find(face) != selectedFaces.end()) {
            continue;
        }
        faceSubSet.push_back(face);
    }

    SurfaceMeshModel *clone = model->clone(faceSubSet);
    model->assign(*clone);
    delete clone;

    model->renderer()->init();

    // 更新缓存数据 UI
    selectionChanged();
    drawArea()->updateGL();
}

/**
 * @brief ModeDeleteFacesPlugin::initConnect
 * 初始化 SIGNAL SLOT 链接
 */
void ModeDeleteFacesPlugin::initConnect()
{
    std::cout << __PRETTY_FUNCTION__ << __LINE__ << std::endl;
    // connect 系统模型改变
    connect(document(), SIGNAL(selectionChanged(Model*)),
            this, SLOT(selectionChanged()), Qt::UniqueConnection);

    connect(deleteFacesWidget->ui->pushButtonDelete, SIGNAL(clicked(bool)),
            this, SLOT(deleteFace()), Qt::UniqueConnection);
}

/**
 * @brief ModeDeleteFacesPlugin::endSelection
 * 基于 QGLViewer 的选择框架
 * @param point
 * @return
 */
bool ModeDeleteFacesPlugin::endSelection(const QPoint &point)
{
    if(currentMode == NONE) {
        return true;
    }

    glFlush();
    GLint nbHits = glRenderMode(GL_RENDER);

    QSet<int> selection;

    if (nbHits > 0) {
        for (int i=0; i< nbHits; ++i) {
            selection.insert((drawArea()->selectBuffer())[4*i+3]);
        }
    }

    for(int idx: selection){
        Face f(idx);
        switch (currentMode) {
        case SELECT:
            // 选择面片
            selectedFaces.insert(f);
            break;
        case DISSELECT:
            // 从集合中移除选择的面片
            if(selectedFaces.find(f) != selectedFaces.end()){
                selectedFaces.erase(selectedFaces.find(f));
            }
            break;
        default:
            break;
        }
    }

    // Update count ui
    Ui::DeleteFacesWidget* ui = deleteFacesWidget->ui;
    ui->faceCounts->setValue(selectedFaces.size());

    return true;
}

/**
 * @brief ModeDeleteFacesPlugin::mouseMoveEvent
 * @return
 */
bool ModeDeleteFacesPlugin::mouseMoveEvent(QMouseEvent *e)
{
    cursorPos = e->pos();
    if(e->modifiers() == Qt::NoButton || currentMode == NONE) return false;

    if(e->buttons() & Qt::LeftButton)   currentMode = SELECT;
    if(e->buttons() & Qt::RightButton)  currentMode = DISSELECT;

    switch(currentMode)
    {
        case SELECT:
            drawArea()->setCursor(paint_cursor);
            drawArea()->select(e->pos());
            isDrawBrushSize = true;
        break;
        case DISSELECT:
            drawArea()->setCursor(erase_cursor);
            drawArea()->select(e->pos());
            isDrawBrushSize = true;
        break;
        case NONE: drawArea()->setCursor(Qt::ArrowCursor); break;
    }

    drawArea()->updateGL();
    return true;
}

/**
 * @brief ModeDeleteFacesPlugin::drawWithNames
 * 给每个三角面片都附加上名字
 * 在 opengl GL_SELECT 模式执行，使用 OpenGL 的鼠标拾取功能
 */
void ModeDeleteFacesPlugin::drawWithNames()
{
    double vt = 0;
    qglviewer::Vec viewDir = drawArea()->camera()->viewDirection().unit();
    Vector3 cameraNormal(viewDir[0],viewDir[1],viewDir[2]);

    for(Face face: mesh()->faces())
    {
        if(dot(fnormals[face], cameraNormal) > vt)
            continue;

        glPushName(face.idx());
        glBegin(GL_POLYGON);
        Surface_mesh::Vertex_around_face_circulator vit, vend;
        vit = vend = mesh()->vertices(face);

        do{
            glVertex3dv(points[vit].data());
        } while(++vit != vend);

        glEnd();
        glPopName();
    }
}

/**
 * @brief ModeDeleteFacesPlugin::decorate
 * 绘制选择的面片
 */
void ModeDeleteFacesPlugin::decorate()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(-10, 10);

    // Selected Faces
    for(Face face: selectedFaces){
        glColor3d(0,1,0);

        glBegin(GL_POLYGON);
        glNormal3dv(fnormals[face].data());
        Surface_mesh::Vertex_around_face_circulator vit, vend;
        vit = vend = mesh()->vertices(face);
        do{ glVertex3dv(points[vit].data()); } while(++vit != vend);
        glEnd();
    }

    glDisable(GL_POLYGON_OFFSET_FILL);
    glEnable(GL_LIGHTING);
}


/**
 * @brief ModeDeleteFacesPlugin::wheelEvent
 * 滚轮行为
 * @return
 */
bool ModeDeleteFacesPlugin::wheelEvent(QWheelEvent *e)
{
    cursorPos = e->pos();
    if(e->modifiers() == Qt::NoButton) return false;

    double s = e->delta() / 120.0;
    brushSize += s;

    drawArea()->setSelectRegionHeight(brushSize);
    drawArea()->setSelectRegionWidth(brushSize);

    isDrawBrushSize = true;
    drawArea()->updateGL();

    return false;
}
