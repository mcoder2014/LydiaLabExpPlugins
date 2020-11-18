#ifndef LAPLACEMINIMALSURFACETOOL_H
#define LAPLACEMINIMALSURFACETOOL_H

#include <vector>
#include <Eigen/Dense>

#include <SurfaceMeshModel.h>

using std::vector;

using SurfaceMesh::SurfaceMeshModel;
using SurfaceMesh::Vertex;
using SurfaceMesh::Halfedge;

using Eigen::Vector3d;
using Eigen::MatrixXd;

class LaplaceMinimalSurfaceTool
{
public:
    LaplaceMinimalSurfaceTool();
    ~LaplaceMinimalSurfaceTool();

    // 将模型转换为极小曲面
    void mininalSurface(SurfaceMeshModel* model);

private:

    void initMatrix(SurfaceMeshModel* model);
    void initValue(SurfaceMeshModel* model);
    void solve();
    void deform(SurfaceMeshModel* model);

    // 内部顶点数目
    int numTotal;

    // 边界顶点数目
    vector<int> boundaryIndex;

    // A * x = B
    // 矩阵 A
    MatrixXd matrixA;

    // 待求解未知数
    MatrixXd pointX;
    MatrixXd pointY;
    MatrixXd pointZ;

    // 右侧参数
    MatrixXd valueX;
    MatrixXd valueY;
    MatrixXd valueZ;
};

#endif // LAPLACEMINIMALSURFACETOOL_H
