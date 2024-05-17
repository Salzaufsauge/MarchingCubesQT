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
    connect(in,&InputWidget::dataExtracted,this,&MainWindow::enableStartBtnSlot);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete in;
    delete out;
}

void MainWindow::loadBtnSlot()
{
    ui->startBtn->setEnabled(false);
    modelUrl = QUrl::fromLocalFile(QFileDialog::getOpenFileName(this,tr("Open Model"),"/home/",tr("Model files (*.obj)")));
    in->addMesh(modelUrl);
}

void MainWindow::enableStartBtnSlot()
{
    ui->startBtn->setEnabled(true);
}
