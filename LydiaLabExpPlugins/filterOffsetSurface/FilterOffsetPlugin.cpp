#include "FilterOffsetPlugin.h"

#include <algorithm/FilpNormals.h>

/**
 * @brief FilterOffsetPlugin::applyFilter
 * 1. 复制两份网格
 * 2. 正向网格正向移动 distance
 * 3. 负向网格负向移动 distance
 * 4. 修改 targetName
 * @param richParameterSet
 */
void FilterOffsetPlugin::applyFilter(RichParameterSet *richParameterSet)
{
    double thinckness = richParameterSet->getFloat("offsetThinckness");
    double distance = thinckness / 2;

    /// 正向移动网格
    SurfaceMeshModel *model = mesh();
    SurfaceMeshModel *positiveMesh = new SurfaceMeshModel;
    positiveMesh->assign(*model);
    positiveMesh->update_vertex_normals();
    offsetAlongVNormals(positiveMesh, distance);
    positiveMesh->updateBoundingBox();
    positiveMesh->update_face_normals();
    positiveMesh->update_vertex_normals();

    /// 负向移动网格
    SurfaceMeshModel *negtiveMesh = new SurfaceMeshModel;
    negtiveMesh->assign(*model);
    filpNormals(negtiveMesh);
    negtiveMesh->update_vertex_normals();
    negtiveMesh->update_face_normals();
    offsetAlongVNormals(negtiveMesh, distance);
    negtiveMesh->updateBoundingBox();
    negtiveMesh->update_face_normals();
    negtiveMesh->update_vertex_normals();

    /// 修改名称 加入场景
    positiveMesh->name = QString("%1_+%2").arg(model->name).arg(distance);
    negtiveMesh->name = QString("%1_-%2").arg(model->name).arg(distance);

    document()->addModel(positiveMesh);
    document()->addModel(negtiveMesh);
}

/**
 * @brief FilterOffsetPlugin::initParameters
 * @param richParameterSet
 */
void FilterOffsetPlugin::initParameters(RichParameterSet *richParameterSet)
{
    // 1. offset 厚度
    richParameterSet->addParam(new RichFloat("offsetThinckness", 2.0, "Thinckness"));
}
