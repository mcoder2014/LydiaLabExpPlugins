#include "FixHoleTool.h"

/**
 * @brief fixHole
 * // 链接对应顶点
 * @param topSurface 上表面
 * @param bottomSurface 下表面
 * @param intervalPoints 插值顶点个数
 * @return
 */
SurfaceMesh::SurfaceMeshModel *fixHole(
        SurfaceMesh::SurfaceMeshModel *topSurface,
        SurfaceMesh::SurfaceMeshModel *bottomSurface,
        int intervalPoints)
{
    // 复制下表面作为目标模型
    SurfaceMeshModel *destModel = new SurfaceMeshModel();
    destModel->assign(*bottomSurface);

    // 用来插入做中间点
    vector<Vertex> topBoundary;
    Surface_mesh::Vertex_property<Vector3d> topPoints = topSurface->vertex_coordinates();

    // 下边界 boundary
    vector<Halfedge> bottomBoundary;
    Surface_mesh::Vertex_property<Vector3d> destPoints = destModel->vertex_coordinates();

    // 上边界 -> 下边界
    map<Vertex, Vertex> top2bottomBoundaryMap;

    // 下边界 -> 新加入的插值节点
    map<Vertex, Vertex> bottom2IntervalVertexMap;

    // build topBoundary build initial top2bottomBoundaryMap
    for(Vertex vertex : topSurface->vertices()) {
        if(topSurface->is_boundary(vertex)){
            topBoundary.push_back(vertex);
            top2bottomBoundaryMap[vertex] = vertex;
        }
    }

    /// 链接中间插值顶点
    for(int i = 0; i < intervalPoints; i++) {
        /// update bottomBoundary
        bottomBoundary.clear();
        for(Halfedge halfedge : destModel->halfedges()) {
            if(destModel->is_boundary(halfedge)) {
                bottomBoundary.push_back(halfedge);
            }
        }

        /// Add interval points Pos = bottom + (i + 1)/(interval + 1) * top
        // update bottom2IntervalPointsMap
        double factor = 1.0 / (intervalPoints + 1);
        bottom2IntervalVertexMap.clear();
        for(Vertex vertex : topBoundary) {
            Vertex destBoundaryVertex = top2bottomBoundaryMap[vertex];
            Vector3d point = destPoints[destBoundaryVertex];
            // 因为上表面边界顶点在 destModel 中的 vertex idx 是不变的
            point = point + factor * (topPoints[vertex] - destPoints[vertex]);

            Vertex intervalVertex = destModel->add_vertex(point);
            bottom2IntervalVertexMap[destBoundaryVertex] = intervalVertex;
        }

        /// 链接下表面和中间节点
        vector<Vertex> face;
        for(Halfedge halfedge : bottomBoundary) {
            // 插入两个face
            Vertex bottomA = destModel->from_vertex(halfedge);
            Vertex bottomB = destModel->to_vertex(halfedge);
            Vertex intervalA = bottom2IntervalVertexMap[bottomA];
            Vertex intervalB = bottom2IntervalVertexMap[bottomB];

            face.clear();
            face.push_back(bottomA);
            face.push_back(bottomB);
            face.push_back(intervalB);
            destModel->add_face(face);

            face.clear();
            face.push_back(bottomA);
            face.push_back(intervalB);
            face.push_back(intervalA);
            destModel->add_face(face);
        }

        /// update top2bottomBoundaryMap
        map<Vertex, Vertex> newTop2bottomBoundaryMap;
        for(std::pair<Vertex, Vertex> pair : top2bottomBoundaryMap) {
            newTop2bottomBoundaryMap[pair.first] = bottom2IntervalVertexMap[pair.second];
        }
       top2bottomBoundaryMap.swap(newTop2bottomBoundaryMap);
    }

    /// Add Top Surface
    // update bottomBoundary
    bottomBoundary.clear();
    for(Halfedge halfedge : destModel->halfedges()) {
        if(destModel->is_boundary(halfedge)) {
            bottomBoundary.push_back(halfedge);
        }
    }

    // add Vertex
    map<Vertex, Vertex> top2newIdxMap;
    bottom2IntervalVertexMap.clear();
    for(Vertex topVertex : topSurface->vertices()) {
        Vertex newVertex = destModel->add_vertex(topPoints[topVertex]);
        top2newIdxMap[topVertex] = newVertex;
        if(topSurface->is_boundary(topVertex)) {
            bottom2IntervalVertexMap[top2bottomBoundaryMap[topVertex]] = newVertex;
        }
    }

    // add Face
    vector<Vertex> topFace;
    for(Face face : topSurface->faces()) {
        topFace.clear();
        for(Vertex vertex : topSurface->vertices(face)) {
            topFace.push_back(top2newIdxMap[vertex]);
        }
        destModel->add_face(topFace);
    }

    // link halfedge
    vector<Vertex> face;
    for(Halfedge halfedge : bottomBoundary) {
        // 插入两个face
        Vertex bottomA = destModel->from_vertex(halfedge);
        Vertex bottomB = destModel->to_vertex(halfedge);
        Vertex intervalA = bottom2IntervalVertexMap[bottomA];
        Vertex intervalB = bottom2IntervalVertexMap[bottomB];

        face.clear();
        face.push_back(bottomA);
        face.push_back(bottomB);
        face.push_back(intervalB);
        destModel->add_face(face);

        face.clear();
        face.push_back(bottomA);
        face.push_back(intervalB);
        face.push_back(intervalA);
        destModel->add_face(face);
    }

    return destModel;
}
