#ifndef MODEDELETEFACESPLUGIN_H
#define MODEDELETEFACESPLUGIN_H

#include <vector>
#include <set>

#include <QMouseEvent>

#include <Eigen/Dense>

#include "interfaces/ModePluginDockWidget.h"
#include "SurfaceMeshPlugins.h"
#include "SurfaceMeshModel.h"
#include "StarlabDrawArea.h"
#include "SurfaceMeshHelper.h"

#include "DeleteFacesWidget.h"

using std::vector;
using std::set;

using SurfaceMesh::SurfaceMeshModel;
using SurfaceMesh::Face;

enum DELETE_FACES_MODE{
    NONE, SELECT, DISSELECT
};

class ModeDeleteFacesPlugin
        :public SurfaceMeshModePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "lydiaLab_mode_delete_faces.plugin.lydialab")
    Q_INTERFACES(ModePlugin)

    // StarlabPlugin interface
public:
    ModeDeleteFacesPlugin();

    QString description() override {
        return tr("Select some faces and delete them.");
    }

    QString name() override {
        return tr("Delete Faces");
    }

    QIcon icon() override {
        return QIcon(":/sources/delete_face.png");
    }

    // 插件是否可用
    bool isApplicable() override {
        return isA(document()->selectedModel());
    }

    // 插件创建时调用
    void create() override;

    // 关闭插件时
    void destroy() override;

    /// UI 交互相关函数
    // 移动鼠标
    bool mouseMoveEvent(QMouseEvent *e) override;
    // 滚轮事件
    bool wheelEvent(QWheelEvent *e) override;
    // 绘制选择的面片
    void drawWithNames() override;
    // 装饰器
    void decorate() override;
    // 选择面片
    bool endSelection(const QPoint &point) override;
    // 监测选择模型切换
    bool selectionChanged();

public slots:
    /// 业务代码
    // 删除面片
    void deleteFace();

private:
    // Dock Widget
    ModePluginDockWidget* modePluginDockWidegt;
    DeleteFacesWidget* deleteFacesWidget;

    /// UI 相关
    // 光标坐标
    QPoint cursorPos;
    // 光标样式
    QCursor paint_cursor, erase_cursor, deform_cursor;
    // 刷子尺寸
    int brushSize;
    // 是否绘制刷子
    bool isDrawBrushSize;
    // 当前插件工作的模式
    DELETE_FACES_MODE currentMode;

    /// 数据相关
    // 选择的面片
    set<Face> selectedFaces;

    /// 缓存
    Vector3VertexProperty points, src_points;
    Vector3FaceProperty fnormals;

private:
    // 初始化 SIGNAL SLOT 链接
    void initConnect();

};

#endif // MODEDELETEFACESPLUGIN_H
