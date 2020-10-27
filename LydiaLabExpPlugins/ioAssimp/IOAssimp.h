#ifndef IOASSIMP_H
#define IOASSIMP_H

#include <vector>

#include "SurfaceMeshPlugins.h"

using std::vector;
class aiMesh;

class IOAssimp
        :public SurfaceMeshInputOutputPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io_assimp.plugin.lydialab")
    Q_INTERFACES(InputOutputPlugin)

public:
    IOAssimp(){}
    virtual ~IOAssimp() override{}

    virtual QString description() override{
        return tr("io using assimp");}
    virtual QString name() override{
        return tr("[io_assimp] Most Objects (*.obj *.ply *.stl)");}

    // InputOutputPlugin interface
public:
    virtual Starlab::Model *open(QString path) override;
    virtual void save(SurfaceMesh::Model *, QString) override;

private:
    int getRepeatVertexIdx(Vector3 & v);
    SurfaceMeshModel *convert(const aiMesh * aimesh);

    // 用来对 STL 格式进行顶点去重
    vector<Vector3> unrepeatVertexList;
    vector<int> uniqueVertexIndexList;

};
#endif // IOASSIMP_H
