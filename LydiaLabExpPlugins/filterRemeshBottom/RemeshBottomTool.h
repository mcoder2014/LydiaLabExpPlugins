#ifndef REMESHBOTTOMTOOL_H
#define REMESHBOTTOMTOOL_H

#include <vector>

#include <Octree.h>

#include <Eigen/Dense>

#include <SurfaceMeshModel.h>

using Eigen::Vector3d;
using Eigen::Matrix4d;

// 将射线转化到模型空间
SurfaceMesh::SurfaceMeshModel *remeshBottomLocal(
        SurfaceMesh::SurfaceMeshModel* srcModel,
        Eigen::Vector3d worldDirection,
        double WorldIntervalX, double WorldIntervalY);

// 用采样点重新构建网格模型
SurfaceMesh::SurfaceMeshModel *remeshBottom(
        std::vector<std::vector<Vector3d>>& sampleGrid,
        Vector3d direction,
        Octree& octree,
        double maxEdge);

// 重新构建模型底部的网格
SurfaceMesh::SurfaceMeshModel *remeshBottom(
        SurfaceMesh::SurfaceMeshModel* srcModel,
        Eigen::Vector3d direction,
        double intervalX, double intervalY);

// 获得坐标系转换的矩阵
Matrix4d getTransformMatrix(Vector3d direction, Vector3d origin);

// 获得采样网格
std::vector<std::vector<Vector3d>> getSampleGrid(
        Eigen::AlignedBox2d boundingBox,
        double intervalX, double intervalY, double depth);

SurfaceMesh::SurfaceMeshModel *debugSampleGrid(std::vector<std::vector<Vector3d>>& sampleGrid);

// 对转移矩阵执行坐标变换
void transform(std::vector<std::vector<Vector3d>>& sampleGrid, Matrix4d transformMatrix);

// 计算二维包围盒
Eigen::AlignedBox2d getBoundingBox2d(Eigen::MatrixXd& pointsMatrix);

// 找到最低的顶点
Eigen::Vector3d getLowestPoint(Eigen::MatrixXd& pointsMatrix);

// 使用 Delaunator 方法重建网格
void addFacesUsingDelaunator(SurfaceMesh::SurfaceMeshModel *model, double maxEdge);

#endif // REMESHBOTTOMTOOL_H
