#include "MirrorTools.h"

#include <algorithm/CoordinateTool.h>

using std::vector;
using Eigen::Vector3d;
using Eigen::Matrix3d;

/**
 * @brief Projection::getTransMatrix
 * 获得坐标系转换矩阵 M 3x3
 * @param face
 * @return
 */
Eigen::Matrix4d getTransMatrix(vector<Eigen::Vector3d> &face)
{
    assert(face.size() >= 3);

    // 三个顶点
    Vector3d& a = face[0];
    Vector3d& b = face[1];
    Vector3d& c = face[2];

    // 两个向量
    Vector3d ba = a - b;
    Vector3d bc = c - b;

    // 利用向量叉乘，获得第一个基向量的 xyz 表示
    Vector3d k = ba.cross(bc);
    k.normalize();

    // 将 ba 向量作为 第二个基向量的 xyz 表示
    Vector3d j = ba;
    j.normalize();

    // 利用向量叉乘求得第三个基向量的 xyz 表示
    Vector3d i = k.cross(j);
    i.normalize();

    return getTransformMatrix(i, j, k, b);
}

/**
 * @brief Projection::getProjectMatrix
 * @param face
 *
 * 投影矩阵
 * M 是空间转换矩阵
 *
 * P' = inv(M)* [1 0 0 0  * M * P
 *              0 1 0 0
 *              0 0 -1 0
 *              0 0 0 1]
 * @return
 */
Eigen::Matrix4d getMiirorMatrix(vector<Eigen::Vector3d> &face)
{
    Matrix4d baseEigen = Matrix4d::Identity();
    baseEigen(2, 2) = -1;

    Eigen::Matrix4d transMat = getTransMatrix(face);
    Eigen::Matrix4d invTransMat = transMat.inverse();

    std::cout << "transMat:\n" << transMat << std::endl;
    std::cout << "invTransMat:\n" << invTransMat << std::endl;

    // 返回投影矩阵
    return invTransMat * baseEigen * transMat;
}


/**
 * @brief mirror
 * @param model
 * @param face
 */
void mirror(SurfaceMesh::SurfaceMeshModel *model, std::vector<Eigen::Vector3d> face)
{
    /// 构建镜像矩阵变换数组
    Eigen::MatrixXd points = Eigen::MatrixXd::Ones(4, model->n_vertices());
    Surface_mesh::Vertex_property<Surface_mesh::Point> vpoints = model->vertex_coordinates();
    for(SurfaceMesh::SurfaceMeshModel::Vertex vertex : model->vertices()) {
        points.block(0, vertex.idx(), 3, 1) = vpoints[vertex];
    }

    // 获得镜像变换的转换矩阵
    Eigen::Matrix4d transformMatrix = getMiirorMatrix(face);
    points = transformMatrix * points;

    /// 翻转面法向量
    SurfaceMesh::SurfaceMeshModel* tmp = new SurfaceMesh::SurfaceMeshModel();
    Surface_mesh::Point point;
    for(int i = 0; i < static_cast<int>(model->n_vertices()); ++i) {
        point = points.block(0, i, 3, 1);
        tmp->add_vertex(point);
    }

    // 复制时翻转顶点顺序
    std::vector<SurfaceMesh::SurfaceMeshModel::Vertex> vFace;
    for(SurfaceMesh::Face f : model->faces()) {
        vFace.clear();
        SurfaceMesh::SurfaceMeshForEachVertexOnFaceHelper vit = model->vertices(f);
        vit = vit.begin();
        SurfaceMesh::SurfaceMeshForEachVertexOnFaceHelper vend = vit;
        for(;vit!=vend;++vit) {
            vFace.push_back(SurfaceMesh::Vertex((*vit).idx()));
        }
        std::reverse(vFace.begin(), vFace.end());

        tmp->add_face(vFace);
    }

    model->Surface_mesh::assign(*tmp);
    delete tmp;
    model->updateBoundingBox();
    model->update_vertex_normals();
    model->update_face_normals();
}
