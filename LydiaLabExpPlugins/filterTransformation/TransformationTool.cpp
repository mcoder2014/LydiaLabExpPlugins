#include "TransformationTool.h"



// 静态变量
const std::string TransformationTool::TRANSFORMATION_PROPERTY_NAME = std::string("Transformation");
const size_t TransformationTool::POSITION_INDEX = 0;
const size_t TransformationTool::ROTATION_INDEX = 1;
const size_t TransformationTool::SCALE_INDEX = 2;

TransformationTool::TransformationTool()
{

}

/**
 * @brief TransformationTool::injectTransformationInfo
 * @param model
 */
void TransformationTool::injectTransformationInfo(SurfaceMesh::SurfaceMeshModel *model)
{
    model->add_vertex_property(TRANSFORMATION_PROPERTY_NAME, Eigen::Vector3d(0.0));
}
