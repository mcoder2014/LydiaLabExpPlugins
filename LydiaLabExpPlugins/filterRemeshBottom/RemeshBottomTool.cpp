#include "RemeshBottomTool.h"

#include <float.h>
#include <map>

#include <Eigen/Geometry>

using Eigen::Vector3d;
using Eigen::Matrix4d;
using SurfaceMesh::SurfaceMeshModel;
using SurfaceMesh::Point;
using std::vector;
using std::map;
using std::pair;

/**
 * @brief remeshBottom
 * Remesh 模型底部算法
 * 1. 变换矩阵，找到射线平面
 * 2. 找到在该射线状态下最低的顶点，稍微再减一点儿，
 * 3. 构建采样面应该在的位置，并在坐标状态构建 interval指定的采样点密度
 * 3. 生成采样点密度矩阵，转换为世界坐标系空间
 * 4. 射线与 octree 模型求交点
 * 5. 无交点，丢弃该采样点
 * 6. 有交点，加入 Remesh 点集 V
 * 7. 对 V 中的顶点进行三角化 构建网格
 *
 * @param srcModel
 * @param direction
 * @param intervalX
 * @param intervalY
 * @return
 */
SurfaceMesh::SurfaceMeshModel *remeshBottom(SurfaceMeshModel* srcModel,
                                            Eigen::Vector3d direction,
                                            double intervalX, double intervalY){
    SurfaceMeshModel::Vertex_property<Point> verticesProperty = srcModel->vertex_coordinates();

    /// 用模型上任一顶点找到转移矩阵 将模型的坐标转换到射线方向所在的坐标系
    Matrix4d transformaMatrix = getTransformMatrix( direction,
                verticesProperty[SurfaceMeshModel::Vertex(0)]);

    // 构造顶点矩阵
    Eigen::MatrixXd pointsMatrix = Eigen::MatrixXd::Ones(4, srcModel->n_vertices());
    for(SurfaceMeshModel::Vertex vertex : srcModel->vertices()) {
        pointsMatrix(0, vertex.idx()) = verticesProperty[vertex].x();
        pointsMatrix(1, vertex.idx()) = verticesProperty[vertex].y();
        pointsMatrix(2, vertex.idx()) = verticesProperty[vertex].z();
    }

    // 执行坐标系转化
    pointsMatrix = transformaMatrix * pointsMatrix;

    // 找到投影方向包围盒
    Eigen::AlignedBox2d boundingBox = getBoundingBox2d(pointsMatrix);
    {
        // 1.2 倍扩充包围盒
        Eigen::Vector2d size = boundingBox.max() - boundingBox.min();
        boundingBox.extend(boundingBox.min() - 0.1 * size);
        boundingBox.extend(boundingBox.max() + 0.1 * size);
    }

    /// 找到最深处顶点的深度 depth， 并在指定在多少深度平面处构建采样网格
    double depth = getLowestPoint(pointsMatrix).z() - 100.0;
    vector<vector<Vector3d>> sampleGrid = getSampleGrid(
                boundingBox, intervalX, intervalY, depth);
    transform(sampleGrid, transformaMatrix.inverse());

    /// 重建网格
    Octree octree(srcModel);
    return remeshBottom(sampleGrid, direction, octree);
}

//bool isZero(double value) {return value < 0.000001 && value > -0.000001;}
/**
 * @brief getTransformMatrix
 * 构建出世界坐标到 以-顶点和以法向量构成的坐标系的转移矩阵 M
 * 齐次坐标形式
 * P' = M * P
 *
 * 1. 根据一个方向向量和原点，生成一个坐标系转换矩阵
 * 2. 求出一个平面方程
 * 3. 在平面上任选一点，组成 OX 向量
 * 4. OZ x OX = XY 计算出 OY 向量
 * 5. OX OY OZ 单位化
 * 6. [OX OY OZ]^T 构建成坐标转换矩阵
 * @param direction
 * @param origin
 * @return
 */
Eigen::Matrix4d getTransformMatrix(Eigen::Vector3d direction, Eigen::Vector3d origin)
{
    // Z方向向量
    Vector3d OZ = direction;

    /// 求解平面方程 ax + by + cz = d
    double a = 0, b = 0, c =0, d = 0;

    a = direction.x();
    b = direction.y();
    c = direction.z();
    d = direction.x() * origin.x() + direction.y() * origin.y() + direction.z() * origin.z();

    /// 从平面上任选一点作为 组件 OX 向量，倾向于选择 X = 0 Y = 0 求解 Z
    Vector3d pointX;
    auto isZero = [](double value)->bool{return value < 0.000001 && value > -0.000001;};

    if(isZero(b) && isZero(c)) {
        // 如果 x 一定不能为 0
        pointX.y() = 0;
        pointX.z() = 0;
        pointX.x() = d / a;
    } else if(isZero(a) && isZero(c)) {
        // 如果 y 一定不能为 0
        pointX.x() = 0;
        pointX.z() = 0;
        pointX.y() = d / b;
    } else {
        // 其余情况，求 z 的值
        pointX.x() = 0;
        pointX.y() = 0;
        pointX.z() = d / c;
    }

    // X 轴
    Vector3d OX = pointX - origin;
    // Y 轴
    Vector3d OY = OZ.cross(OX);

    // 正则化
    OX.normalize();
    OY.normalize();
    OZ.normalize();

    Matrix4d transformMatrix = Matrix4d::Identity();
    transformMatrix.topLeftCorner(3,3) << OX, OY, OZ;
    return transformMatrix.transpose().inverse();
}

/**
 * @brief getSampleGrid
 * 生成采样网格
 * 1. 计算
 * 在当前坐标系按指定方式生成采样网格
 * @param boundingBox
 * @param intervalX
 * @param intervalY
 * @param depth
 * @return
 */
std::vector<std::vector<Eigen::Vector3d> > getSampleGrid(
        Eigen::AlignedBox2d boundingBox,
        double intervalX, double intervalY, double depth)
{
    /// 计算采样矩阵的尺寸
    Eigen::Vector2d boundingBoxSize = boundingBox.max() - boundingBox.min();
    Eigen::Vector2i samplePointCount(
                static_cast<int>(boundingBoxSize.x() / intervalX),
                static_cast<int>(boundingBoxSize.y() / intervalY));

    // 初始化采样矩阵
    vector<vector<Vector3d>> samplePoints(static_cast<size_t>(samplePointCount.x()),
        vector<Vector3d>(static_cast<size_t>(samplePointCount.y()), Vector3d::Ones()));

    Eigen::Vector3d tmpPoint;
    tmpPoint << boundingBox.min(), depth;

    // 初始化采样点的坐标
    for(size_t i = 0; i < static_cast<size_t>(samplePointCount.x()); i++) {
        for(size_t j = 0; j < static_cast<size_t>(samplePointCount.y()); j++) {
            samplePoints[i][j] = tmpPoint;
            tmpPoint.y() += intervalY;
        }
        tmpPoint.x() += intervalX;
        tmpPoint.y() = boundingBox.min().y();
    }

    return samplePoints;
}

/**
 * @brief transform
 * @param sampleGrid
 * @param transformMatrix
 */
void transform(std::vector<std::vector<Eigen::Vector3d> > &sampleGrid, Eigen::Matrix4d transformMatrix)
{
    size_t countX = sampleGrid.size();
    size_t countY = sampleGrid[0].size();

    Eigen::MatrixXd pointsMatrix = Eigen::MatrixXd::Ones(4, static_cast<int>(countX * countY));

    size_t idx;
    for(size_t i = 0; i < countX; i++) {
        for(size_t j = 0; j < countY; j++) {
            idx = i * countY + j;
            pointsMatrix(0, static_cast<int>(idx)) = sampleGrid[i][j].x();
            pointsMatrix(1, static_cast<int>(idx)) = sampleGrid[i][j].y();
            pointsMatrix(2, static_cast<int>(idx)) = sampleGrid[i][j].z();
        }
    }

    pointsMatrix = transformMatrix * pointsMatrix;

    for(size_t i = 0; i < countX; i++) {
        for(size_t j = 0; j < countY; j++) {
            idx = i * countY + j;
            sampleGrid[i][j].x() = pointsMatrix(0, static_cast<int>(idx));
            sampleGrid[i][j].y() = pointsMatrix(1, static_cast<int>(idx));
            sampleGrid[i][j].z() = pointsMatrix(2, static_cast<int>(idx));
        }
    }
}

/**
 * @brief getBoundingBox2d
 * @param pointsMatrix
 * @return
 */
Eigen::AlignedBox2d getBoundingBox2d(Eigen::MatrixXd &pointsMatrix)
{
    Eigen::AlignedBox2d boungdingBox;
    int size = static_cast<int>(pointsMatrix.cols());
    Eigen::Vector2d tmpPoint;
    for(int i = 0; i < size; i++) {
        tmpPoint.x() = pointsMatrix(0, i);
        tmpPoint.y() = pointsMatrix(1, i);
        boungdingBox.extend(tmpPoint);
    }
    return boungdingBox;
}

/**
 * @brief getLowestPoint
 * 找到最低的顶点
 * @param pointsMatrix
 * @return
 */
Eigen::Vector3d getLowestPoint(Eigen::MatrixXd &pointsMatrix)
{
    double minimum = static_cast<double>(pointsMatrix(2, 0));
    int idx = 0;

    int size = static_cast<int>(pointsMatrix.cols());
    Eigen::Vector2d tmpPoint;
    for(int i = 0; i < size; i++) {
        if(pointsMatrix(2, i) < minimum) {
            minimum = pointsMatrix(2, i);
            idx = i;
        }
    }

    Eigen::Vector3d lowestPoint;
    lowestPoint.x() = pointsMatrix(0, idx);
    lowestPoint.y() = pointsMatrix(1, idx);
    lowestPoint.z() = pointsMatrix(2, idx);

    return lowestPoint;
}

/**
 * @brief addFace
 * 辅助函数，往模型中添加顶点
 * 1. 需要指定顶点已经存在于模型中才可添加面片
 * @param model
 * @param i
 * @param j
 * @param indexMap
 */
void addFace(SurfaceMeshModel *model, int i, int j, map<pair<int, int>, int>& indexMap){

    /// lambda 查找指定顶点的编号
    auto getVertexIndex = [&indexMap](int i, int j) {
        if(indexMap.find(pair<int, int>(i, j)) != indexMap.end()) {
            return indexMap[pair<int, int>(i, j)];
        }
        return -1;
    };

    int idxA = getVertexIndex(i, j);
    int idxB = getVertexIndex(i, j + 1);
    int idxC = getVertexIndex(i + 1, j);
    int idxD = getVertexIndex(i + 1, j + 1);

    /// 添加面片
    vector<SurfaceMeshModel::Vertex> face;
    auto addFace = [&face, model](int a, int b, int c) {
        face.clear();
        if(a != -1 && b != -1 && c != -1) {
            face.push_back(SurfaceMeshModel::Vertex(a));
            face.push_back(SurfaceMeshModel::Vertex(b));
            face.push_back(SurfaceMeshModel::Vertex(c));
            model->add_face(face);
        }
    };

    addFace(idxD, idxB, idxA);
    addFace(idxC, idxD, idxA);
}

/**
 * @brief remeshBottom
 * 将采样矩阵的顶点沿着方向求交点，remesh
 * 1. 对每个顶点与 Octree 求交点
 * 2. 有交点的会加入remesh 的网格中
 * 3. 没有交点的顶点会被淘汰
 * 4. 新建 SurfaceMeshModel
 * @param sampleGrid
 * @param direction
 * @param octree
 * @return
 */
SurfaceMesh::SurfaceMeshModel *remeshBottom(
        std::vector<std::vector<Eigen::Vector3d> > &sampleGrid,
        Eigen::Vector3d direction,
        Octree &octree)
{
    SurfaceMeshModel *model = new SurfaceMeshModel;

    // 记录 grid 顶点对应的索引
    map<pair<int, int>, int> indexMap;

    size_t countX = sampleGrid.size();
    size_t countY = sampleGrid[0].size();
    int faceIndex;

    for(size_t i = 0; i < countX; i++) {
        for(size_t j = 0; j < countY; j++) {
            Ray ray(sampleGrid[i][j], direction, 0.000001);
            faceIndex = -1;
            Vector3d intersectPoint = octree.closestIntersectionPoint(ray, &faceIndex);
            if(faceIndex > 0) {
                // 有交点的面片加入网格
                SurfaceMeshModel::Vertex vertex = model->add_vertex(intersectPoint);
                indexMap[pair<int, int>(i, j)] = vertex.idx();
            }
        }
    }

    // 建立网格链接
    for(int i = 0; i < static_cast<int>(countX - 1); i++) {
        for(int j = 0; j < static_cast<int>(countY - 1); j++) {
            addFace(model, i, j, indexMap);
        }
    }

    return model;
}
/**
 * @brief debugSampleGrid
 * DEBUG 目的
 * 将采样点矩阵生成 SurfaceMeshModel
 * @param sampleGrid
 * @return
 */
SurfaceMesh::SurfaceMeshModel *debugSampleGrid(std::vector<std::vector<Eigen::Vector3d> > &sampleGrid)
{
    SurfaceMeshModel *model = new SurfaceMeshModel;

    // 记录 grid 顶点对应的索引
    map<pair<int, int>, int> indexMap;
    Vector3d startPoint;

    size_t countX = sampleGrid.size();
    size_t countY = sampleGrid[0].size();
    for(size_t i = 0; i < countX; i++) {
        for(size_t j = 0; j < countY; j++) {
            startPoint = sampleGrid[i][j];
            SurfaceMeshModel::Vertex vertex = model->add_vertex(startPoint);
            indexMap[pair<int, int>(i, j)] = vertex.idx();
        }
    }

    // 建立网格链接
    for(int i = 0; i < static_cast<int>(countX - 1); i++) {
        for(int j = 0; j < static_cast<int>(countY - 1); j++) {
            addFace(model, i, j, indexMap);
        }
    }

    return model;
}
