#ifndef IOOPENMESHPLUGIN_H
#define IOOPENMESHPLUGIN_H

#include "SurfaceMeshPlugins.h"

class IOOpenmeshPlugin
        :public SurfaceMeshInputOutputPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io_openmesh.plugin.lydialab")
    Q_INTERFACES(InputOutputPlugin)

public:
    IOOpenmeshPlugin();

    // StarlabPlugin interface
public:
    virtual QString name() override{
        return tr("[io_openmesh] openmesh support formats (*.obj *.stl *.ply *.off)");
    }
    virtual QString description() override{
        return tr("io supported by openmesh");
    }

    // InputOutputPlugin interface
public:
    virtual Starlab::Model *open(QString path) override;
    virtual void save(SurfaceMesh::Model *model, QString path) override;
};

#endif // IOOPENMESHPLUGIN_H
