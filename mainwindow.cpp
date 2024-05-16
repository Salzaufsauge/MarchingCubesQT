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
    connect( ui->loadButton,&QPushButton::clicked,this, &MainWindow::loadButtonSlot);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete in;
    delete out;
}

void MainWindow::loadButtonSlot()
{
    ui->startButton->setEnabled(false);
    modelUrl = QUrl::fromLocalFile(QFileDialog::getOpenFileName(this,tr("Open Model"),"/home/",tr("Model files (*.obj)")));
    in->addMesh(modelUrl);
    ui->startButton->setEnabled(true);
}
