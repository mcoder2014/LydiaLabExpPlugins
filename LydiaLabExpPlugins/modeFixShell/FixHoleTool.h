#ifndef FIXHOLETOOL_H
#define FIXHOLETOOL_H

#include <vector>
#include <map>

#include <Eigen/Dense>

#include <SurfaceMeshModel.h>

using std::vector;
using std::map;
using Eigen::Vector3d;
using SurfaceMesh::SurfaceMeshModel;
using SurfaceMesh::Halfedge;
using SurfaceMesh::Vertex;
using SurfaceMesh::Face;

// 连接上下两层网格(上下网格的顶点序对应)
SurfaceMeshModel *fixHole(
        SurfaceMeshModel* topSurface,
        SurfaceMeshModel* bottomSurface,
        int intervalPoints = 0);

#endif // FIXHOLETOOL_H
