#pragma once

#include <QObject>
#include <QtWidgets>
#include <QtCore>
#include <Qt3DCore/Qt3DCore>
#include <Qt3DRender/Qt3DRender>
#include <Qt3DInput/Qt3DInput>
#include <Qt3DExtras/Qt3DExtras>

class Mc3DViewer
{
public:
    Mc3DViewer(QVBoxLayout *vboxLayout);
    ~Mc3DViewer();
private:
    void initCamera();
protected:
    QWidget *widget;
    Qt3DCore::QEntity *rootEntity;
    Qt3DCore::QEntity *objectEntity;
    Qt3DExtras::QGoochMaterial *material;
    Qt3DExtras::Qt3DWindow *view;
    Qt3DRender::QCamera *inputCam;
    Qt3DCore::QEntity *lightEntity;
    Qt3DRender::QPointLight *light;
    Qt3DCore::QTransform *lightTransform;
    Qt3DExtras::QFirstPersonCameraController *camController;
};
