#pragma once

#include <QMainWindow>
#include <QtCore>
#include <QtWidgets>
#include <Qt3DCore/Qt3DCore>
#include <Qt3DRender/Qt3DRender>
#include <Qt3DInput/Qt3DInput>
#include <Qt3DExtras/Qt3DExtras>
#include <memory>

#include "inputwidget.hpp"
#include "outputwidget.hpp"
#include "scalarfield.hpp"
#include "marchingcontroller.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

Q_DECLARE_METATYPE(QList<Eigen::Vector3f>);

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    std::unique_ptr<Grid> grid;

    Ui::MainWindow *ui;
    QUrl modelUrl;
    InputWidget *in;
    OutputWidget *out;
    ScalarField data;
    void disableUi();
    void uiModelLoaded();
    void uiSFLoaded();
    QList<Vector3f> vertices;
    QList<uint> indices;
    MarchingController *marchingController;
    MarchingFlags getSelectedFlag();
    int getFlags();


private slots:
    void loadBtnSlot();
    void modelLoadedSlot();
    void sfBtnSlot();
    void resChangedSlot();
    void polyBtnSlot();
    void speedSliderSlot();
    void previewSlot(QList<Vector3f> vertices, QList<uint> indices);
};
