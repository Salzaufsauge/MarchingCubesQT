#pragma once

#include <QObject>
#include <QtWidgets>
#include <QtCore>
#include <Qt3DCore/Qt3DCore>
#include <Qt3DRender/Qt3DRender>
#include <Qt3DInput/Qt3DInput>
#include <Qt3DExtras/Qt3DExtras>

class Mc3DViewer : public QWidget
{
public:
    Mc3DViewer(QWidget *parent = nullptr,QVBoxLayout *vboxLayout = nullptr);
protected:
    Qt3DCore::QEntity *rootEntity;
    Qt3DExtras::QGoochMaterial *material;
    Qt3DExtras::Qt3DWindow *view;

};
