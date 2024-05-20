#include "outputwidget.hpp"

OutputWidget::OutputWidget(QVBoxLayout *vboxLayout)
    :Mc3DViewer(vboxLayout)
{

}

void OutputWidget::buildMesh(const QList<Vector3f> &vertices, const QList<uint> &indices)
{
    mesh->buildMesh(vertices, indices);
    objectEntity->addComponent(mesh);
}

