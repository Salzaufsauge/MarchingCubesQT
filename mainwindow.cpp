#include "mainwindow.hpp"
#include "./ui_mainwindow.h"
#include "marchingcubes.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    in = new InputWidget(ui->inputLayout);
    out = new OutputWidget(ui->outputLayout);

    connect( ui->loadBtn,&QPushButton::clicked,this, &MainWindow::loadBtnSlot);
    connect(in->getMesh(),&Mesh::dataExtracted,this,&MainWindow::enableStartBtnSlot);
    connect(ui->startBtn,&QPushButton::clicked,this, &MainWindow::startBtnSlot);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete in;
    delete out;
}

void MainWindow::changeUIState()
{
    uiState = !uiState;
    ui->startBtn->setEnabled(uiState);
    ui->loadBtn->setEnabled(uiState);
    ui->resolutionSlider->setEnabled(uiState);
    ui->speedSlider->setEnabled(uiState);
    ui->addonBox->setEnabled(uiState);
}

void MainWindow::loadBtnSlot()
{
    ui->startBtn->setEnabled(false);
    modelUrl = QUrl::fromLocalFile(QFileDialog::getOpenFileName(this,
                                                                tr("Open Model"),
                                                                QStandardPaths::writableLocation(QStandardPaths::DownloadLocation),
                                                                tr("Model files (*.obj)")));
    if(modelUrl.isEmpty()){
        qDebug() << "No model loaded";
        return;
    }
    in->addMesh(modelUrl);
}

void MainWindow::enableStartBtnSlot()
{
    ui->startBtn->setEnabled(true);
}

void MainWindow::startBtnSlot()
{
    uint res = std::pow(2,ui->resolutionSlider->value());
    changeUIState();
    vertices.clear();
    indices.clear();
    vertices.squeeze();
    indices.squeeze();
    QFuture<void> future = QtConcurrent::run([this, res]() {
        Grid grid;
        grid.constructGrid(res,in->getMesh()->getMaxExtend(),in->getMesh()->getMinExtend());
// #pragma omp parallel for
//         for(auto i = 0; i < grid.getRes().x();++i)
//             for(auto j = 0; j < grid.getRes().y();++j)
//                 for(auto k = 0; k < grid.getRes().z();++k)
//                     grid.setSdfAt(i,j,k,data.sdTorus(grid.getPoints()[i][j][k],Vector2f(0.5, 0.5)));
        data.calculateSDF(grid, in->getMesh()->getVertices(), in->getMesh()->getIndices());
        MarchingCubes mCubes;
        mCubes.mc(grid,0,vertices,indices);
    });

    QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);
    connect(watcher, &QFutureWatcher<void>::finished, this, [watcher, this]() {
        watcher->deleteLater();
        changeUIState();
        out->buildMesh(vertices,indices);
    });
    watcher->setFuture(future);
}
