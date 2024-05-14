#include "mainwindow.hpp"
#include "./ui_mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    inputView = new Qt3DExtras::Qt3DWindow();
    outputView = new Qt3DExtras::Qt3DWindow();

    inputView->defaultFrameGraph()->setClearColor(QColor(Qt::gray));
    outputView->defaultFrameGraph()->setClearColor(QColor(Qt::gray));
    inputWidget = QWidget::createWindowContainer(inputView);
    outputWidget = QWidget::createWindowContainer(outputView);

    inputWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    inputWidget->setFocusPolicy(Qt::ClickFocus);
    outputWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    outputWidget->setFocusPolicy(Qt::ClickFocus);

    ui->inputLayout->addWidget(inputWidget);
    ui->outputLayout->addWidget(outputWidget);

    inputRoot = new Qt3DCore::QEntity();
    inputMesh = new Qt3DCore::QEntity(inputRoot);

    Qt3DExtras::QGoochMaterial *material = new Qt3DExtras::QGoochMaterial;
    material->setDiffuse(Qt::red);
    inputMesh->addComponent(material);

    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform;
    transform->setScale3D(QVector3D(5,5,5));

    Qt3DRender::QCamera *inputCam = inputView->camera();
    inputCam->lens()->setPerspectiveProjection(50.0f,16.0f/9.0f,0.1f,1000.0f);
    inputCam->setPosition(QVector3D(0,0,10.0f));
    inputCam->setUpVector(QVector3D(0,1,0));
    inputCam->setViewCenter(QVector3D(0,0,0));

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(inputRoot);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(inputCam->position());
    lightEntity->addComponent(lightTransform);

    Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(inputRoot);
    camController->setCamera(inputCam);

    inputView->setRootEntity(inputRoot);

    connect( ui->loadButton,&QPushButton::clicked,this, &MainWindow::loadButtonSlot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadButtonSlot()
{
    if(newMesh != nullptr){
        newMesh = nullptr;
        delete newMesh;
    }
    ui->startButton->setEnabled(false);
    modelUrl = QUrl::fromLocalFile(QFileDialog::getOpenFileName(this,tr("Open Model"),"/home/",tr("Model files (*.obj)")));
    newMesh = new Qt3DRender::QMesh;
    newMesh->setSource(modelUrl);
    inputMesh->addComponent(newMesh);
    ui->startButton->setEnabled(true);
    inputWidget->setFocus();
}
