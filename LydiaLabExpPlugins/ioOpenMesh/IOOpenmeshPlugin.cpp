#include "IOOpenmeshPlugin.h"

#include <OpenMesh/Core/IO/MeshIO.hh>

#include "Mesh.h"
#include "Convertor.h"

IOOpenmeshPlugin::IOOpenmeshPlugin()
{

}

/**
 * @brief IOOpenmeshPlugin::open
 * @param path
 * @return
 */
Starlab::Model *IOOpenmeshPlugin::open(QString path)
{
    Mesh mesh;
    OpenMesh::IO::read_mesh(mesh, path.toStdString());

    Convertor convertor;
    SurfaceMesh::SurfaceMeshModel* model = convertor.mesh2SurfaceMeshModel(mesh);

    // mesh name
    QFileInfo fileinfo(path);
    model->name = fileinfo.baseName();
    model->path = fileinfo.absolutePath();

    return model;
}

/**
 * @brief IOOpenmeshPlugin::save
 * @param path
 */
void IOOpenmeshPlugin::save(Model * model, QString path)
{
    SurfaceMeshModel* surfaceMeshModel = SurfaceMesh::safeCast(model);
    Convertor convertor;
    Mesh mesh = convertor.surfaceMeshModel2mesh(*surfaceMeshModel);

    OpenMesh::IO::write_mesh(mesh, path.toStdString());
}
