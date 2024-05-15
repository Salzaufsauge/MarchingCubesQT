#include "mc3dviewer.hpp"

Mc3DViewer::Mc3DViewer(QVBoxLayout *vboxLayout) {
    view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor::fromRgb(255,255,255));

    widget = QWidget::createWindowContainer(view);
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    widget->setFocusPolicy(Qt::StrongFocus);
    vboxLayout->addWidget(widget);

    rootEntity = new Qt3DCore::QEntity;
    initCamera();
    objectEntity = new Qt3DCore::QEntity(rootEntity);

    material = new Qt3DExtras::QGoochMaterial;
    material->setDiffuse(Qt::red);
    objectEntity->addComponent(material);

    view->setRootEntity(rootEntity);
}

void Mc3DViewer::initCamera()
{
    Qt3DRender::QCamera *inputCam = view->camera();
    inputCam->lens()->setPerspectiveProjection(50.0f,16.0f/9.0f,0.1f,1000.0f);
    inputCam->setPosition(QVector3D(0,0,10.0f));
    inputCam->setUpVector(QVector3D(0,1,0));
    inputCam->setViewCenter(QVector3D(0,0,0));

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);

    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);

    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(inputCam->position());
    lightEntity->addComponent(lightTransform);

    Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
    camController->setCamera(inputCam);
}
