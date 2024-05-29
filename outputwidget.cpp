#include "outputwidget.hpp"

OutputWidget::OutputWidget(QVBoxLayout *vboxLayout)
    :Mc3DViewer(vboxLayout)
{
    previewMesh = new Mesh(objectEntity);
}

void OutputWidget::buildMesh(const QList<Vector3f> &vertices, const QList<uint> &indices)
{
    mesh->buildMesh(vertices, indices);
    objectEntity->addComponent(mesh);
}

void OutputWidget::buildPreviewMesh(const QList<Vector3f> &vertices, const QList<uint> &indices)
{
    if(objectEntity->componentsOfType<Mesh>().isEmpty()){
        previewMesh->buildMesh(vertices,indices,false);
        objectEntity->addComponent(previewMesh);
    }else {
        previewMesh->buildMesh(vertices,indices,false);
    }
}

void OutputWidget::buildWireframe(const QList<QList<Vector3f> > &cells)
{
    // for(auto &cell : cells){

    // }
}

void OutputWidget::clearMesh()
{
    if(!objectEntity->componentsOfType<Mesh>().isEmpty())
        objectEntity->removeComponent(mesh);
}

void OutputWidget::clearPreview()
{
    // for(auto cell : wireframe){
    //     objectEntity->removeComponent(cell);
    //     delete cell;
    // }
    // wireframe.clear();
    objectEntity->removeComponent(previewMesh);
}

