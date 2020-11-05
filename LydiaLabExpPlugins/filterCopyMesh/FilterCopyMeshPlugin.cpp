#include "FilterCopyMeshPlugin.h"

#include "SurfaceMeshModel.h"

using SurfaceMesh::SurfaceMeshModel;

/**
 * @brief FilterCopyMeshPlugin::applyFilter
 * @param richParameterSet
 */
void FilterCopyMeshPlugin::applyFilter(RichParameterSet *richParameterSet)
{
    QString newName = document()->selectedModel()->name + "_clone";
    if(richParameterSet->hasParameter("newName")){
        newName = richParameterSet->getString("newName");
    }

    SurfaceMeshModel* model = safeCast(document()->selectedModel());
    SurfaceMeshModel* newModel = new SurfaceMeshModel;
    newModel->assign(*model);
    newModel->name = newName;

    document()->addModel(newModel);
}

/**
 * @brief FilterCopyMeshPlugin::initParameters
 * @param richParameterSet
 */
void FilterCopyMeshPlugin::initParameters(RichParameterSet *richParameterSet)
{
    richParameterSet->addParam(
                new RichString("newName",
                               document()->selectedModel()->name+"_clone",
                               "New Nesh Name"));
}
