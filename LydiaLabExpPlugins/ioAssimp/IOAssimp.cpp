#include "IOAssimp.h"

#include "assimp/Importer.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/**
 * @brief convert
 * @param aimesh
 * @return
 */
SurfaceMeshModel* IOAssimp::convert(const aiMesh * aimesh) {

    SurfaceMeshModel* mesh = new SurfaceMeshModel();

    std::cout << "faces: " << aimesh->HasFaces() << "\t"
              << aimesh->mNumFaces << "\n"
              << "vertices: " << aimesh->mNumVertices << std::endl;

    // Add vertex and faces into the mesh
    size_t length_vertices = aimesh->mNumVertices;
    size_t length_faces = aimesh->mNumFaces;
    aiVector3D *aivertices = aimesh->mVertices;
    aiFace *aifaces = aimesh->mFaces;

    // Compare the speed after inport Flann
    {
        // Old way to remove duplicate vertex
        clock_t start_time = clock();

        std::vector<SurfaceMesh::Vertex> face;  // Used to add each face
        this->uniqueVertexIndexList.resize(length_vertices);

        // 添加顶点
        for (size_t i=0; i < length_vertices; ++i){
            Vector3 vi(static_cast<double>(aivertices[i].x),
                       static_cast<double>(aivertices[i].y),
                       static_cast<double>(aivertices[i].z));
            // 去掉重复顶点
            int index_v0 = getRepeatVertexIdx(vi);

            if( index_v0 == -1){
                // 当前是个新节点
                mesh->add_vertex(vi);
                unrepeatVertexList.push_back(vi);
                index_v0 = static_cast<int>(unrepeatVertexList.size() - 1);
            }
            uniqueVertexIndexList[i] = index_v0;
        }

        std::cout << "After unified, there are "
                  << unrepeatVertexList.size() << std::endl;

        // 添加面片
        for (size_t i=0; i < length_faces; ++i)
        {
            face.clear();
            for (size_t vec_indices=0; vec_indices < aifaces[i].mNumIndices; ++vec_indices)
            {
                face.push_back(SurfaceMesh::Vertex(static_cast<int>(
                    uniqueVertexIndexList[aifaces[i].mIndices[vec_indices]])));
            }

            // load insole file error
            mesh->add_face(face);
        }
        clock_t end_time = clock();
        std::cout << "Old approach took " << 1.0 * (end_time-start_time)/CLOCKS_PER_SEC << "S" << std::endl;
    }

    mesh->update_vertex_normals();
    mesh->update_face_normals();
    mesh->updateBoundingBox();
    return mesh;
}

/**
 * @brief IOAssimp::open
 * @param path
 * @return
 */
Starlab::Model *IOAssimp::open(QString path)
{
    this->unrepeatVertexList.clear();
    this->uniqueVertexIndexList.clear();

    Assimp::Importer importer;
    SurfaceMeshModel *mesh = nullptr;

    try {

        const aiScene *scene = importer.ReadFile(
                    path.toStdString(),aiProcess_Triangulate);

        if(scene == nullptr || scene->mNumMeshes <= 0 ){
            return new SurfaceMeshModel();
        }

        // Read mesh Only load the first mesh;
        mesh = convert(scene->mMeshes[0]);
    }
    catch (...)
    {
        std::cerr << "Load file: " << path.toStdString() << " failed.\n"
                  << importer.GetErrorString() << std::endl;
        return new SurfaceMeshModel();
    }

    // mesh name
    QFileInfo fileinfo(path);
    mesh->name = fileinfo.baseName();
    mesh->path = fileinfo.absolutePath();

    return mesh;
}

void IOAssimp::save(Model *, QString )
{
    mainWindow()->setStatusBarMessage(tr("Assimp not support for export!"));
}

/**
 * @brief IOAssimp::getRepeatVertexIdx
 * @param v
 * @return
 */
int IOAssimp::getRepeatVertexIdx(Vector3 &v)
{
    int idx = -1;
    if(unrepeatVertexList.size() == 0) return idx;
    for (size_t i = unrepeatVertexList.size() - 1; i != 0; i--)
    {
        if (std::abs(unrepeatVertexList[i][0] - v[0]) < 0e-5
            && std::abs(unrepeatVertexList[i][1] - v[1]) < 0e-5
            && std::abs(unrepeatVertexList[i][2] - v[2]) < 0e-5)
        {
            idx = static_cast<int>(i);
            break;
        }
    }
    return idx;
}
