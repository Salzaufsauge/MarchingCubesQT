#pragma once

#include <QMainWindow>
#include <QtCore>
#include <QtWidgets>
#include <Qt3DCore/Qt3DCore>
#include <Qt3DRender/Qt3DRender>
#include <Qt3DInput/Qt3DInput>
#include <Qt3DExtras/Qt3DExtras>

#include "inputwidget.hpp"
#include "outputwidget.hpp"
#include "scalarfield.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    bool uiState = true;

    Ui::MainWindow *ui;
    QUrl modelUrl;
    InputWidget *in;
    OutputWidget *out;
    ScalarField data;
    void changeUIState();

private slots:
    void loadBtnSlot();
    void enableStartBtnSlot();
    void startBtnSlot();
};
