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
    bool calculateNormals = false;
#ifdef _WIN32
        calculateNormals = true;
#endif
    if(objectEntity->componentsOfType<Mesh>().isEmpty()){
        previewMesh->buildMesh(vertices,indices,calculateNormals);
        objectEntity->addComponent(previewMesh);
    }else {
        previewMesh->buildMesh(vertices,indices,calculateNormals);
    }
}

void OutputWidget::clearMesh()
{
    if(!objectEntity->componentsOfType<Mesh>().isEmpty())
        objectEntity->removeComponent(mesh);
}

void OutputWidget::clearPreview()
{
    objectEntity->removeComponent(previewMesh);
}

