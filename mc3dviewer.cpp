#include "mc3dviewer.hpp"

Mc3DViewer::Mc3DViewer(QVBoxLayout *vboxLayout,QWidget* parent)
    :QWidget(parent),widget(nullptr),rootEntity(nullptr), objectEntity(nullptr), material(nullptr), view(nullptr)
{
    view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor::fromRgb(100,100,100));

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

Mc3DViewer::~Mc3DViewer()
{
    delete rootEntity;
    delete view;

}

void Mc3DViewer::initCamera()
{
    inputCam = view->camera();
    inputCam->lens()->setPerspectiveProjection(50.0f,16.0f/9.0f,0.1f,1000.0f);
    inputCam->setPosition(QVector3D(0,0,10.0f));
    inputCam->setUpVector(QVector3D(0,1,0));
    inputCam->setViewCenter(QVector3D(0,0,0));

    lightEntity = new Qt3DCore::QEntity(rootEntity);

    light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);

    lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(inputCam->position());
    lightEntity->addComponent(lightTransform);

    camController = new Qt3DExtras::QFirstPersonCameraController(rootEntity);
    camController->setCamera(inputCam);
}
