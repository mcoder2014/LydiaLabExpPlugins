#include "FilterRemeshBottomPlugin.h"

#include "RemeshBottomTool.h"

/**
 * @brief FilterRemeshBottomPlugin::applyFilter
 * @param richParameterSet
 */
void FilterRemeshBottomPlugin::applyFilter(RichParameterSet *richParameterSet)
{
    if(richParameterSet == nullptr
            || document()->selectedModel() == nullptr ||
            !isA(document()->selectedModel())) {
        throw new StarlabException("param is illegal");
    }

    /// 获得参数
    double intervalX = static_cast<double>(richParameterSet->getFloat("intervalX"));
    double intervalY = static_cast<double>(richParameterSet->getFloat("intervalY"));

    Vector3d direction;
    direction.x() = static_cast<double>(richParameterSet->getFloat("diectX"));
    direction.y() = static_cast<double>(richParameterSet->getFloat("diectY"));
    direction.z() = static_cast<double>(richParameterSet->getFloat("diectZ"));

    /// 应用重建方法
    SurfaceMeshModel* model =  remeshBottomLocal(mesh(), direction, intervalX, intervalY);

    model->name = QString("%1_d_%2_%3_%4_i_%5_%6")
            .arg(document()->selectedModel()->name)
            .arg(direction.x())
            .arg(direction.y())
            .arg(direction.z())
            .arg(intervalX)
            .arg(intervalY);
    model->setRenderer(nullptr);
    model->updateBoundingBox();
    document()->addModel(model);
}

/**
 * @brief FilterRemeshBottomPlugin::initParameters
 * @param richParameterSet
 */
void FilterRemeshBottomPlugin::initParameters(RichParameterSet *richParameterSet)
{
    // 1. 采样点密度
    richParameterSet->addParam(new RichFloat("intervalX", 1.0, "Interval.x"));
    richParameterSet->addParam(new RichFloat("intervalY", 1.0, "Interval.y"));

    // 2. 采样的射线方向
    richParameterSet->addParam(new RichFloat("diectX", 0.0, "Direct Vector.x"));
    richParameterSet->addParam(new RichFloat("diectY", 0.0, "Direct Vector.y"));
    richParameterSet->addParam(new RichFloat("diectZ", 1.0, "Direct Vector.z"));
}
