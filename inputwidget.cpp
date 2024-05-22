#include "inputwidget.hpp"

InputWidget::InputWidget(QVBoxLayout *vboxLayout)
    :Mc3DViewer(vboxLayout)
{
    connect(mesh,&Mesh::statusChanged,this,&InputWidget::meshStatusChanged);
}

void InputWidget::addMesh(QUrl url)
{
    mesh->setSource(url);
    objectEntity->addComponent(mesh);
}

void InputWidget::meshStatusChanged(Qt3DRender::QMesh::Status newStatus)
{
    if(newStatus == Qt3DRender::QMesh::Status::Ready){
        mesh->extractMeshData();
    }
}
