#include "TransformationTool.h"

using Point = Surface_mesh::Point;
using VertexProperty = Surface_mesh::Vertex_property<Point>;
using Vertex = Surface_mesh::Vertex;

/**
 * @brief transform
 * @param mesh
 * @param transformMatrix
 */
void transformation(Surface_mesh *mesh, Eigen::Matrix4d transformMatrix)
{
    // 计算顶点矩阵
    Eigen::MatrixXd points = Eigen::MatrixXd::Ones(4, static_cast<int>(mesh->n_vertices()));

    VertexProperty vpoints = mesh->get_vertex_property<Point>(SurfaceMesh::VPOINT);
    for(Surface_mesh::Vertex_iterator iter = mesh->vertices_begin();
        iter != mesh->vertices_end(); ++iter) {
        Vertex vertex = Vertex(iter);
        points(0, vertex.idx()) = vpoints[vertex].x();
        points(1, vertex.idx()) = vpoints[vertex].y();
        points(2, vertex.idx()) = vpoints[vertex].z();
    }

    points = transformMatrix * points;

    for(Surface_mesh::Vertex_iterator iter = mesh->vertices_begin();
        iter != mesh->vertices_end(); ++iter) {
        Vertex vertex = Vertex(iter);
        vpoints[vertex].x() = points(0, vertex.idx());
        vpoints[vertex].y() = points(1, vertex.idx());
        vpoints[vertex].z() = points(2, vertex.idx());
    }

}

/**
 * @brief setModelOrigin
 * 修改模型局部坐标的中心点
 * @param model
 * @param newOrigin
 */
void setModelOrigin(SurfaceMesh::SurfaceMeshModel *model, Eigen::Vector3d newOrigin)
{
    VertexProperty vpoints = model->vertex_coordinates();
    for(Vertex vertex : model->vertices()) {
        vpoints[vertex] = vpoints[vertex] - newOrigin;
    }
    model->updateBoundingBox();
}

/**
 * @brief equal
 * 判断两个顶点是否相等
 * @param v1
 * @param v2
 * @return
 */
bool equal(const Eigen::Vector3d &v1, const Eigen::Vector3d &v2)
{
    double ZERO = 0e-6;
    return fabs(v1.x() - v2.x()) < ZERO
            && fabs(v1.y() - v2.y()) < ZERO
            && fabs(v1.z() - v2.z()) < ZERO;
}
