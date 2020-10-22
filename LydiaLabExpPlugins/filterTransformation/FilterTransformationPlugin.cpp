#include "FilterTransformationPlugin.h"

#include <iostream>

#include <Eigen/Dense>
#include <parameters/RichParameterWidget.h>

#include "TransformRichParameterWidget.h"
#include "TransformationTool.h"

/**
 * @brief FilterTransformationPlugin::initParameters
 * @param richParameterSet
 */
void FilterTransformationPlugin::initParameters(RichParameterSet *richParameterSet)
{
    richParameterSet->addParam(
                new TransformRichParameter("Transform", "desc", "tooltip"));
    std::cout << __PRETTY_FUNCTION__ << __LINE__ << std::endl;
}

/**
 * @brief FilterTransformationPlugin::applyFilter
 * @param richParameterSet
 */
void FilterTransformationPlugin::applyFilter(RichParameterSet *richParameterSet)
{
    std::cout << __PRETTY_FUNCTION__ << __LINE__ << std::endl;
}

/**
 * @brief FilterTransformationPlugin::isApplicable
 * 可使用条件，非空
 * @param model
 * @return
 */
bool FilterTransformationPlugin::isApplicable(Starlab::Model *model)
{
    if(model == nullptr)
        return false;

    // 转换为 model
    SurfaceMeshModel* surfaceMeshModel = safeCast(model);
    if(!TransformationTool::checkTransformationInfo(surfaceMeshModel)) {
        TransformationTool::injectTransformationInfo(surfaceMeshModel);
    }
}
