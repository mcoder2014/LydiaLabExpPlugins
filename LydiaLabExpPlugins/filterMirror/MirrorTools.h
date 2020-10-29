#ifndef MIRRORTOOLS_H
#define MIRRORTOOLS_H

#include <vector>

#include <SurfaceMeshModel.h>
#include <Eigen/Dense>

void mirror(SurfaceMesh::SurfaceMeshModel* model, std::vector<Eigen::Vector3d> face);
#endif // MIRRORTOOLS_H
