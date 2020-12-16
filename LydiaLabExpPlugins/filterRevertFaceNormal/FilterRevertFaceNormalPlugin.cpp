#include "FilterRevertFaceNormalPlugin.h"

#include <SurfaceMeshModel.h>
#include <algorithm/FilpNormals.h>

void FilterRevertFaceNormalPlugin::applyFilter(RichParameterSet *)
{
    SurfaceMesh::SurfaceMeshModel* model = safeCast(document()->selectedModel());

    filpNormals(model);
}
