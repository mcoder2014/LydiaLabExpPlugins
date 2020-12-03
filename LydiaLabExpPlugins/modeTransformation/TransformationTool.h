#ifndef TRANSFORMATIONTOOL_H
#define TRANSFORMATIONTOOL_H

#include <string>

#include <Eigen/Dense>

#include <SurfaceMeshModel.h>

using Eigen::Vector3d;
using SurfaceMesh::SurfaceMeshModel;

/**
 * @brief transform
 * 对模型执行变换
 * @param mesh
 * @param transformMatrix
 */
void transformation(Surface_mesh* mesh, Eigen::Matrix4d transformMatrix);

// 修改模型局部坐标的中心点
void setModelOrigin(SurfaceMeshModel* model, Eigen::Vector3d newOrigin);

bool equal(const Eigen::Vector3d &v1, const Eigen::Vector3d &v2);

#endif // TRANSFORMATIONTOOL_H
