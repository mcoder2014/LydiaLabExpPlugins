#ifndef TRANSFORMATIONTOOL_H
#define TRANSFORMATIONTOOL_H

#include <string>

#include <Eigen/Dense>

#include <SurfaceMeshModel.h>

using Eigen::Vector3d;

/**
 * @brief transform
 * 对模型执行变换
 * @param mesh
 * @param transformMatrix
 */
void transformation(Surface_mesh* mesh, Eigen::Matrix4d transformMatrix);

#endif // TRANSFORMATIONTOOL_H
