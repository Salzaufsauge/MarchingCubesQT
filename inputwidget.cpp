#include "inputwidget.hpp"

InputWidget::InputWidget(QVBoxLayout *vboxLayout)
    :Mc3DViewer(vboxLayout)
{
}

void InputWidget::addMesh(QUrl url)
{
    mesh.setSource(url);
    objectEntity->addComponent(&mesh);
}
