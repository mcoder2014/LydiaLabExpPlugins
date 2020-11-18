#include "FilterMinimalSurfacePlugin.h"

#include "LaplaceMinimalSurfaceTool.h"

using SurfaceMesh::Vertex;

void FilterMinimalSurfacePlugin::applyFilter(RichParameterSet *)
{
    SurfaceMeshModel *model = safeCast(document()->selectedModel());
    SurfaceMeshModel *newModel = new SurfaceMeshModel;
    newModel->assign(*model);

    LaplaceMinimalSurfaceTool laplaceMinmalSurfaceTool;
    laplaceMinmalSurfaceTool.mininalSurface(newModel);
    newModel->updateBoundingBox();
    newModel->update_face_normals();
    newModel->update_vertex_normals();

    document()->addModel(newModel);
}

bool FilterMinimalSurfacePlugin::isApplicable(Starlab::Model *model)
{
    if(model == nullptr || !isA(model)) {
        return false;
    }

    SurfaceMeshModel *mesh = safeCast(model);

    for(Vertex vertex : mesh->vertices()) {
        if(mesh->is_boundary(vertex)) {
            return true;
        }
    }

    return false;
}
