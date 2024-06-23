#pragma once

#include <QtCore>
#include <Qt3DCore/Qt3DCore>
#include <Qt3DRender/Qt3DRender>
#include <Qt3DInput/Qt3DInput>
#include <Qt3DExtras/Qt3DExtras>

class WireframeMaterial : public Qt3DRender::QMaterial
{
public:
    WireframeMaterial(Qt3DCore::QNode *parent = nullptr);
private:
    QColor diffuse;
    QColor specular;
    QColor ambient;
    float shininess;

};
