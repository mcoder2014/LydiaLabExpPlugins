#ifndef TRANSFORMRICHPARAMETERWIDGET_H
#define TRANSFORMRICHPARAMETERWIDGET_H

#include <Eigen/Dense>

#include "Model.h"
#include "SurfaceMeshModel.h"

#include "ui_TransformationWidget.h"

using Eigen::Vector3d;
using Eigen::AlignedBox3d;

#define PI 3.1415926

/**
 * @brief The TransformRichParameterWidget class
 * UI 相关
 */
class TransformationWidget :public QWidget, public Ui::TransformationWidget
{
    Q_OBJECT

public:
    explicit TransformationWidget(QWidget *parent);
    ~TransformationWidget();

    // 更新界面数据
    void update(SurfaceMesh::Model* model);

    // 显示空白数据
    void setDefault();

    void updateUI(
            Vector3d position, Vector3d rotation,
            Vector3d scale, AlignedBox3d bbox);

    inline double rad2ang(const double& rad) {return rad * 180 / PI;}
    inline double ang2rad(const double& ang) {return ang * PI / 180;}
};



#endif // TRANSFORMRICHPARAMETERWIDGET_H
