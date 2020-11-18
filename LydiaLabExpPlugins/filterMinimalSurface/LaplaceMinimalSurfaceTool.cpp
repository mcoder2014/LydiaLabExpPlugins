#include "LaplaceMinimalSurfaceTool.h"

LaplaceMinimalSurfaceTool::LaplaceMinimalSurfaceTool()
    :numTotal(0)
{

}

LaplaceMinimalSurfaceTool::~LaplaceMinimalSurfaceTool()
{

}

/**
 * @brief LaplaceMinimalSurfaceTool::mininalSurface
 * 构建极小曲面
 * @param model
 */
void LaplaceMinimalSurfaceTool::mininalSurface(SurfaceMesh::SurfaceMeshModel *model)
{
    initMatrix(model);

    initValue(model);

    solve();

    deform(model);
}

void LaplaceMinimalSurfaceTool::initMatrix(SurfaceMesh::SurfaceMeshModel *model)
{
    // Matrix
    for(Vertex vertex:model->vertices()) {
        if(model->is_boundary(vertex)) {
            boundaryIndex.push_back(vertex.idx());
        }
        numTotal++;
    }

    matrixA = MatrixXd::Zero((static_cast<int>(boundaryIndex.size()) + numTotal), numTotal);

    /// init matrix L
    for(int i = 0; i < numTotal; i++) {
        double value = -1.0 / model->valence(Vertex(i));
        for(Halfedge halfedge : model->onering_hedges(Vertex(i))) {
            matrixA(i, model->to_vertex(halfedge).idx()) = value;
        }
        matrixA(i, i) = 1;
    }

    /// init matrix F
    for(size_t i = 0; i < boundaryIndex.size(); i++) {
        matrixA(numTotal + static_cast<int>(i), boundaryIndex[i]) = 1;
    }
}

void LaplaceMinimalSurfaceTool::initValue(SurfaceMesh::SurfaceMeshModel *model)
{
    pointX = MatrixXd::Zero(numTotal, 1);
    pointY = MatrixXd::Zero(numTotal, 1);
    pointZ = MatrixXd::Zero(numTotal, 1);

    valueX = MatrixXd::Zero(numTotal + static_cast<int>(boundaryIndex.size()), 1);
    valueY = MatrixXd::Zero(numTotal + static_cast<int>(boundaryIndex.size()), 1);
    valueZ = MatrixXd::Zero(numTotal + static_cast<int>(boundaryIndex.size()), 1);

    Surface_mesh::Vertex_property<Vector3d> points = model->vertex_coordinates();

    // 固定点
    for(size_t i = 0; i < boundaryIndex.size(); i++) {
        Vector3d point = points[Vertex(boundaryIndex[i])];
        valueX(numTotal + static_cast<int>(i), 0) = point.x();
        valueY(numTotal + static_cast<int>(i), 0) = point.y();
        valueZ(numTotal + static_cast<int>(i), 0) = point.z();
    }
}

/**
 * @brief LaplaceMinimalSurfaceTool::solve
 * 解方程
 */
void LaplaceMinimalSurfaceTool::solve()
{
    Eigen::HouseholderQR<MatrixXd> qr;
    qr.compute(matrixA);
    pointX = qr.solve(valueX);
    pointY = qr.solve(valueY);
    pointZ = qr.solve(valueZ);
}

/**
 * @brief LaplaceMinimalSurfaceTool::deform
 * @param model
 * 应用变化结果
 */
void LaplaceMinimalSurfaceTool::deform(SurfaceMeshModel* model)
{
    Surface_mesh::Vertex_property<Vector3d> points = model->vertex_coordinates();
    for(Vertex vertex : model->vertices()) {
        Vector3d point = points[vertex];
        point.x() = pointX(vertex.idx(), 0);
        point.y() = pointY(vertex.idx(), 0);
        point.z() = pointZ(vertex.idx(), 0);
    }
}
