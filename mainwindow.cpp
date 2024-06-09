#include "mainwindow.hpp"
#include "./ui_mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    in = new InputWidget(ui->inputLayout);
    out = new OutputWidget(ui->outputLayout);
    marchingController = new MarchingController(this);
    grid = std::make_unique<Grid>();

    disableUi();
    ui->loadBtn->setVisible(true);

    ui->mcRadioBtn->setProperty("flag",QVariant::fromValue(MarchingFlags::MARCHING_CUBES));
    ui->dualRadioBtn->setProperty("flag",QVariant::fromValue(MarchingFlags::DUAL_MARCHING));
    ui->mtRadioBtn->setProperty("flag", QVariant::fromValue(MarchingFlags::MARCHING_TETRAHEDA));
    ui->smoothBox->setProperty("flag",QVariant::fromValue(MarchingFlags::SMOOTHING));

    connect( ui->loadBtn,&QPushButton::clicked,this, &MainWindow::loadBtnSlot);
    connect(in->getMesh(),&Mesh::dataExtracted,this,&MainWindow::modelLoadedSlot);
    connect(ui->sfBtn,&QPushButton::clicked,this, &MainWindow::sfBtnSlot);
    connect(ui->polyBtn, &QPushButton::clicked,this,&MainWindow::polyBtnSlot);
    connect(ui->resolutionSlider, &QSlider::valueChanged,this,&MainWindow::resChangedSlot);
    connect(ui->speedSlider, &QSlider::valueChanged,this,&MainWindow::speedSliderSlot);
    connect(marchingController,&MarchingController::newData,this,&MainWindow::previewSlot);
    connect(ui->conBtn,&QPushButton::clicked,this,&MainWindow::conBtnSlot);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete in;
    delete out;
}

void MainWindow::disableUi()
{
    ui->statusEdit->setVisible(false);
    ui->statusLabel->setVisible(false);
    ui->loadBtn->setVisible(false);
    ui->addonBox->setVisible(false);
    ui->polyBtn->setVisible(false);
    ui->sfBtn->setVisible(false);
    ui->speedSlider->setVisible(false);
    ui->speedLabel->setVisible(false);
    ui->resolutionLabel->setVisible(false);
    ui->resolutionSlider->setVisible(false);
    ui->bvhBox->setVisible(false);
    ui->isoSlider->setVisible(false);
    ui->isoLabel->setVisible(false);
    ui->conBtn->setVisible(false);
}

void MainWindow::uiModelLoaded()
{
    ui->statusEdit->setVisible(false);
    ui->statusLabel->setVisible(false);
    ui->loadBtn->setVisible(true);
    ui->sfBtn->setVisible(true);
    ui->resolutionLabel->setVisible(true);
    ui->resolutionSlider->setVisible(true);
    ui->bvhBox->setVisible(true);
}

void MainWindow::uiSFLoaded()
{
    ui->statusEdit->setVisible(false);
    ui->statusLabel->setVisible(false);
    ui->loadBtn->setVisible(true);
    ui->sfBtn->setVisible(false);
    ui->resolutionLabel->setVisible(true);
    ui->resolutionSlider->setVisible(true);
    ui->speedLabel->setVisible(true);
    ui->speedSlider->setVisible(true);
    ui->polyBtn->setVisible(true);
    ui->addonBox->setVisible(true);
    ui->isoSlider->setVisible(true);
    ui->isoLabel->setVisible(true);
    ui->conBtn->setVisible(true);
}

MarchingFlags MainWindow::getSelectedFlag()
{
    if(ui->mcRadioBtn->isChecked()) return ui->mcRadioBtn->property("flag").value<MarchingFlags>();
    else if(ui->dualRadioBtn->isChecked()) return ui->dualRadioBtn->property("flag").value<MarchingFlags>();
    else if(ui->mtRadioBtn->isChecked()) return ui->mtRadioBtn->property("flag").value<MarchingFlags>();
    return MarchingFlags();
}

int MainWindow::getFlags()
{
    int flags = static_cast<int>(getSelectedFlag());
    if(ui->smoothBox->isChecked()) flags |= static_cast<int>(ui->smoothBox->property("flag").value<MarchingFlags>());
    return flags;
}

void MainWindow::loadBtnSlot()
{
    disableUi();
    QUrl newModelUrl = QUrl::fromLocalFile(QFileDialog::getOpenFileName(this,
                                                                tr("Open Model"),
                                                                QStandardPaths::writableLocation(QStandardPaths::DownloadLocation),
                                                                tr("Model files (*.obj)")));
    if(newModelUrl.isEmpty()){
        qDebug() << "No model loaded";
        ui->loadBtn->setVisible(true);
        if(in->getMesh()->geometry() != nullptr) uiModelLoaded();
        return;
    }
    if(newModelUrl == modelUrl){
        qDebug() << "Model already loaded";
        uiModelLoaded();
        return;
    }
    modelUrl = newModelUrl;
    in->addMesh(modelUrl);

}

void MainWindow::modelLoadedSlot()
{
    uiModelLoaded();
}

void MainWindow::sfBtnSlot()
{
    uint res = std::pow(2,ui->resolutionSlider->value());
    disableUi();
    ui->statusLabel->setVisible(true);
    ui->statusEdit->setVisible(true);

    vertices.clear();
    indices.clear();
    vertices.squeeze();
    indices.squeeze();

    out->clearMesh();

    QFuture<void> future = QtConcurrent::run([this, res]() {
        grid = std::make_unique<Grid>();
        ui->statusEdit->setText(QString("constructing grid"));
        grid->constructGrid(res,in->getMesh()->getMaxExtend(),in->getMesh()->getMinExtend());
        ui->statusEdit->setText(QString("calculating scalar field"));
        if(ui->bvhBox->isChecked())
            data.calculateSDFBVH(*grid, in->getMesh()->getVertices(), in->getMesh()->getIndices());
        else
            data.calculateSDF(*grid, in->getMesh()->getVertices(), in->getMesh()->getIndices());
    });

    QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);
    connect(watcher, &QFutureWatcher<void>::finished, this, [watcher, this]() {
        watcher->deleteLater();
        uiSFLoaded();
    });
    watcher->setFuture(future);
}

void MainWindow::resChangedSlot()
{
    disableUi();
    uiModelLoaded();
}

void MainWindow::polyBtnSlot()
{
    disableUi();
    ui->speedLabel->setVisible(true);
    ui->speedSlider->setVisible(true);
    ui->statusLabel->setVisible(true);
    ui->statusEdit->setVisible(true);

    vertices.clear();
    indices.clear();
    vertices.squeeze();
    indices.squeeze();

    out->clearMesh();

    QFuture<void> future = QtConcurrent::run([this]() {
        ui->statusEdit->setText(QString("calculating polygons"));
        marchingController->calculatePolygons(*grid,static_cast<float>(ui->isoSlider->value()) / 100.0,vertices,indices,getFlags());
    });

    QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);
    connect(watcher, &QFutureWatcher<void>::finished, this, [watcher, this]() {
        watcher->deleteLater();
        out->clearPreview();
        ui->statusEdit->setText("building mesh");
        out->buildMesh(vertices,indices);
        uiSFLoaded();
    });
    watcher->setFuture(future);
}

void MainWindow::conBtnSlot()
{
    disableUi();
    for(float i = -10000.0;i <= 10000.0; i++){
        vertices.clear();
        indices.clear();
        vertices.squeeze();
        indices.squeeze();

        marchingController->calculatePolygons(*grid,i / 10000.0,vertices,indices,static_cast<int>(MarchingFlags::MARCHING_CUBES));
        if(!vertices.isEmpty()){
            out->clearMesh();
            out->buildMesh(vertices,indices);
            QCoreApplication::processEvents();
            QThread::msleep(5);
        }
    }
    out->clearMesh();
    uiSFLoaded();
}

void MainWindow::speedSliderSlot()
{
    marchingController->setSpeed(1000 - (ui->speedSlider->value()));
}

void MainWindow::previewSlot(QList<Vector3f> previewVertices, QList<uint> previewIndices)
{
    if(marchingController->getRunning())
        out->buildPreviewMesh(previewVertices,previewIndices);
}
