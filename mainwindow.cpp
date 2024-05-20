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
    QFuture<void> future = QtConcurrent::run([this, res]() {
        in->constructGrid(res);
        data.calculateSDF(in->getGrid(), in->getMesh()->getVertices(), in->getMesh()->getIndices());
    });

    QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);
    connect(watcher, &QFutureWatcher<void>::finished, this, [watcher, this]() {
        watcher->deleteLater();
        changeUIState();
    });
    watcher->setFuture(future);
}
