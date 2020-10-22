#ifndef TRANSFORMATIONTOOL_H
#define TRANSFORMATIONTOOL_H

#include <string>

#include <Eigen/Dense>

#include <SurfaceMeshModel.h>

using Eigen::Vector3d;

class TransformationTool
{
public:
    TransformationTool();

    // 隐藏在 SurfaceMeshModel 的某个属性中
    static const std::string TRANSFORMATION_PROPERTY_NAME;
    static const size_t POSITION_INDEX;
    static const size_t ROTATION_INDEX;
    static const size_t SCALE_INDEX;

    // 给一个模型注入位移信息
    static void injectTransformationInfo(SurfaceMesh::SurfaceMeshModel *model);

    // 检查模型是否含有位移信息
    static bool checkTransformationInfo(SurfaceMesh::SurfaceMeshModel *model){
        return model->has_vertex_property<Vector3d>(TRANSFORMATION_PROPERTY_NAME);
    }
};

#endif // TRANSFORMATIONTOOL_H
