#include "FilterMirrorPlugin.h"

#include <vector>

#include <Eigen/Dense>
#include <StarlabException.h>

#include "MirrorTools.h"

FilterMirrorPlugin::FilterMirrorPlugin()
{

}

void FilterMirrorPlugin::applyFilter(RichParameterSet *richParameterSet)
{
    std::vector<Eigen::Vector3d> face(3, Eigen::Vector3d::Zero());
    Starlab::Model *model = document()->selectedModel();
    // 获得参数
    if(richParameterSet == nullptr
            || model == nullptr
            || !isA(model)){
        throw new StarlabException("Exception:");
        return;
    }

    face[0].x() = static_cast<double>(richParameterSet->getFloat("V0_x"));
    face[0].y() = static_cast<double>(richParameterSet->getFloat("V0_y"));
    face[0].z() = static_cast<double>(richParameterSet->getFloat("V0_z"));

    face[1].x() = static_cast<double>(richParameterSet->getFloat("V1_x"));
    face[1].y() = static_cast<double>(richParameterSet->getFloat("V1_y"));
    face[1].z() = static_cast<double>(richParameterSet->getFloat("V1_z"));

    face[2].x() = static_cast<double>(richParameterSet->getFloat("V2_x"));
    face[2].y() = static_cast<double>(richParameterSet->getFloat("V2_y"));
    face[2].z() = static_cast<double>(richParameterSet->getFloat("V2_z"));

    // 执行镜像操作
    mirror(safeCast(model), face);
}

/**
 * @brief FilterMirrorPlugin::initParameters
 * 初始化参数界面
 * @param richParameterSet
 */
void FilterMirrorPlugin::initParameters(RichParameterSet *richParameterSet)
{
    richParameterSet->addParam(new RichFloat("V0_x", 0.0, "X of Vertex[0]"));
    richParameterSet->addParam(new RichFloat("V0_y", 0.0, "Y of Vertex[0]"));
    richParameterSet->addParam(new RichFloat("V0_z", 0.0, "Z of Vertex[0]"));
    richParameterSet->addParam(new RichFloat("V1_x", 0.0, "X of Vertex[1]"));
    richParameterSet->addParam(new RichFloat("V1_y", 1.0, "Y of Vertex[1]"));
    richParameterSet->addParam(new RichFloat("V1_z", 0.0, "Z of Vertex[1]"));
    richParameterSet->addParam(new RichFloat("V2_x", 0.0, "X of Vertex[2]"));
    richParameterSet->addParam(new RichFloat("V2_y", 0.0, "Y of Vertex[2]"));
    richParameterSet->addParam(new RichFloat("V2_z", 1.0, "Z of Vertex[2]"));
}

bool FilterMirrorPlugin::isApplicable(Starlab::Model *model)
{
    return model != nullptr && isA(model);
}
