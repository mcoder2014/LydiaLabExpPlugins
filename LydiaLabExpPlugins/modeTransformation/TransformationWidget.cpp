#include "TransformationWidget.h"
#include "ui_TransformationWidget.h"

#include <iostream>

#include "TransformationTool.h"

using namespace SurfaceMesh;

/**
 * @brief TransformationWidget::TransformationWidget
 * 1. 构建UI
 * 2. 建立链接
 * @param parent
 */
TransformationWidget::TransformationWidget(
        QWidget *parent)
            :QWidget(parent), ui(new Ui::TransformationWidget)
{
    // 构建 UI 样式
    ui->setupUi(this);

    // 当点击按钮时，发射应用变换信号
    connect(ui->pushButtonAccept, SIGNAL(clicked(bool)),
            this, SLOT(emitTransformationChanged()));
}

TransformationWidget::~TransformationWidget()
{
    delete ui;
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
    ui->doubleSpinBoxPosX->setValue(position.x());
    ui->doubleSpinBoxPosY->setValue(position.y());
    ui->doubleSpinBoxPosZ->setValue(position.z());

    ui->doubleSpinBoxRotX->setValue(rad2ang(rotation.x()));
    ui->doubleSpinBoxRotY->setValue(rad2ang(rotation.y()));
    ui->doubleSpinBoxRotZ->setValue(rad2ang(rotation.z()));

    Vector3d extension = bbox.max() - bbox.min();

    ui->doubleSpinBoxBboxX->setValue(extension.x());
    ui->doubleSpinBoxBboxY->setValue(extension.y());
    ui->doubleSpinBoxBboxZ->setValue(extension.z());

    ui->doubleSpinBoxScaleX->setValue(scale.x());
    ui->doubleSpinBoxScaleY->setValue(scale.y());
    ui->doubleSpinBoxScaleZ->setValue(scale.z());
}

void TransformationWidget::emitTransformationChanged()
{
    Vector3d position;
    Vector3d rotation;
    Vector3d scale;
    Vector3d size;

    position.x() = ui->doubleSpinBoxPosX->value();
    position.y() = ui->doubleSpinBoxPosY->value();
    position.z() = ui->doubleSpinBoxPosZ->value();

    rotation.x() = ang2rad(ui->doubleSpinBoxRotX->value());
    rotation.y() = ang2rad(ui->doubleSpinBoxRotY->value());
    rotation.z() = ang2rad(ui->doubleSpinBoxRotZ->value());

    scale.x() = ui->doubleSpinBoxScaleX->value();
    scale.y() = ui->doubleSpinBoxScaleY->value();
    scale.z() = ui->doubleSpinBoxScaleZ->value();

    size.x() = ui->doubleSpinBoxBboxX->value();
    size.y() = ui->doubleSpinBoxBboxY->value();
    size.z() = ui->doubleSpinBoxBboxZ->value();

    emit transformationChagned(position, rotation, scale, size);
}

