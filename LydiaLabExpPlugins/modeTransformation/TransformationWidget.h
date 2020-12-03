#ifndef TRANSFORMRICHPARAMETERWIDGET_H
#define TRANSFORMRICHPARAMETERWIDGET_H

#include <Eigen/Dense>

#include "Model.h"
#include "SurfaceMeshModel.h"

using Eigen::Vector3d;
using Eigen::AlignedBox3d;

#define PI 3.1415926

namespace Ui {
class TransformationWidget;
}

/**
 * @brief The TransformRichParameterWidget class
 * UI 相关
 */
class TransformationWidget :public QWidget
{
    Q_OBJECT

public:
    explicit TransformationWidget(QWidget *parent);
    ~TransformationWidget();

    Ui::TransformationWidget *ui;

    // 更新界面数据
    void update(SurfaceMesh::Model* model);

    // 显示空白数据
    void setDefault();

    void updateUI(
            Vector3d position, Vector3d rotation,
            Vector3d scale, AlignedBox3d bbox);

private:


    inline double rad2ang(const double& rad) {return rad * 180 / PI;}
    inline double ang2rad(const double& ang) {return ang * PI / 180;}

signals:
    // ui 更改 transformation
    void transformationChagned(
            Vector3d position, Vector3d rotation,
            Vector3d scale, Vector3d size);

public slots:
    // 发送位置变换信号
    void emitTransformationChanged();
};



#endif // TRANSFORMRICHPARAMETERWIDGET_H
