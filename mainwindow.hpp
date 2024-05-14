#pragma once

#include <QMainWindow>
#include <QtCore>
#include <QtWidgets>
#include <Qt3DCore/Qt3DCore>
#include <Qt3DRender/Qt3DRender>
#include <Qt3DInput/Qt3DInput>
#include <Qt3DExtras/Qt3DExtras>

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
    Ui::MainWindow *ui;
    QUrl modelUrl;
    Qt3DCore::QEntity *inputMesh;
    Qt3DCore::QEntity *inputRoot;
    Qt3DExtras::Qt3DWindow *inputView;
    Qt3DExtras::Qt3DWindow *outputView;
    QWidget *inputWidget;
    QWidget *outputWidget;
    Qt3DRender::QMesh *newMesh;

private slots:
    void loadButtonSlot();
};
