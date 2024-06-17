#pragma once

#include "mesh.hpp"
#include "wireframematerial.hpp"
#include <QObject>
#include <QtWidgets>
#include <QtCore>
#include <Qt3DCore/Qt3DCore>
#include <Qt3DRender/Qt3DRender>
#include <Qt3DInput/Qt3DInput>
#include <Qt3DExtras/Qt3DExtras>

class Mc3DViewer : public QWidget
{
    Q_OBJECT
public:
    Mc3DViewer(QVBoxLayout *vboxLayout, QWidget* parent = nullptr);
    ~Mc3DViewer();
    Mesh *getMesh() const;
private:
    void initCamera();
protected:
    QWidget *widget;
    Qt3DCore::QEntity *rootEntity;
    Qt3DCore::QEntity *objectEntity;
    WireframeMaterial *material;
    Qt3DExtras::Qt3DWindow *view;
    Qt3DRender::QCamera *camera;
    Qt3DCore::QEntity *lightEntity;
    Qt3DRender::QPointLight *light;
    Qt3DCore::QTransform *lightTransform;
    Qt3DExtras::QFirstPersonCameraController *camController;
    Mesh *mesh;
};
