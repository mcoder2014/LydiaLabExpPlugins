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

SurfaceMeshModel *fixHole(
        SurfaceMeshModel* topSurface,
        SurfaceMeshModel* bottomSurface,
        int intervalPoints = 0);

#endif // FIXHOLETOOL_H
