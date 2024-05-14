#include "mc3dviewer.hpp"

Mc3DViewer::Mc3DViewer(QWidget *parent,QVBoxLayout *vboxLayout) {
    view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor::fromRgb(255,255,255));
    parent = QWidget::createWindowContainer(view);
    vboxLayout->addWidget(parent);
    rootEntity = new Qt3DCore::QEntity;
    material = new Qt3DExtras::QGoochMaterial;
    material->setDiffuse(Qt::red);
}
