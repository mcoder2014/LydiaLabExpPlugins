#include "TransformationWidget.h"


#include <iostream>

#include "TransformationTool.h"

using namespace SurfaceMesh;

/**
 * @brief TransformationWidget::TransformationWidget
 * 1. 构建UI
 * 2. 建立链接
 * @param parent
 */
TransformationWidget::TransformationWidget(QWidget *parent) :QWidget(parent)
{
    // 构建 UI 样式
    setupUi(this);
}

TransformationWidget::~TransformationWidget()
{
}

/**
 * @brief TransformationWidget::update
 * 更新显示内容为指定模型的数据
 * @param model
 */
void TransformationWidget::update(SurfaceMesh::Model *model)
{
//    std::cout << "Update UI " << __FILE__ << __PRETTY_FUNCTION__ << std::endl;
    if(model == nullptr || !isA(model)) {
        setDefault();
        return;
    }

    // 更新 UI
    SurfaceMeshModel* surfacemeshModel = safeCast(model);
    AlignedBox3d boundingBox = surfacemeshModel->bbox();

    updateUI(surfacemeshModel->getPosition(),
             surfacemeshModel->getRotation(),
             surfacemeshModel->getScale(),
             boundingBox);
}

/**
 * @brief TransformationWidget::setDefault
 * 设置默认显示状态
 */
void TransformationWidget::setDefault()
{
    updateUI(Vector3d::Zero(),
             Vector3d::Zero(),
             Vector3d::Zero(),
             AlignedBox3d());
}

/**
 * @brief TransformationWidget::updateUI
 * 1. 更底层的更新显示数据
 * 2. 需要进行角度与弧度值的转换
 * @param position
 * @param rotation
 * @param scale
 * @param bbox
 */
void TransformationWidget::updateUI(
        Eigen::Vector3d position,
        Eigen::Vector3d rotation,
        Eigen::Vector3d scale,
        Eigen::AlignedBox3d bbox)
{
    doubleSpinBoxPosX->setValue(position.x());
    doubleSpinBoxPosY->setValue(position.y());
    doubleSpinBoxPosZ->setValue(position.z());

    doubleSpinBoxRotX->setValue(rad2ang(rotation.x()));
    doubleSpinBoxRotY->setValue(rad2ang(rotation.y()));
    doubleSpinBoxRotZ->setValue(rad2ang(rotation.z()));

    Vector3d extension = bbox.max() - bbox.min();

    doubleSpinBoxBboxX->setValue(extension.x() * scale.x());
    doubleSpinBoxBboxY->setValue(extension.y() * scale.y());
    doubleSpinBoxBboxZ->setValue(extension.z() * scale.z());

    doubleSpinBoxScaleX->setValue(scale.x());
    doubleSpinBoxScaleY->setValue(scale.y());
    doubleSpinBoxScaleZ->setValue(scale.z());
}
