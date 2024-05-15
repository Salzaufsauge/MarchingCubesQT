#include "inputwidget.hpp"

InputWidget::InputWidget(QVBoxLayout *vboxLayout)
    :Mc3DViewer(vboxLayout)
{
    mesh = new Qt3DRender::QMesh;
}

void InputWidget::addMesh(QUrl url)
{
    mesh->setSource(url);
    objectEntity->addComponent(mesh);
}
