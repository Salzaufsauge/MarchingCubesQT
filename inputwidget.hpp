#pragma once

#include "mc3dviewer.hpp"

class InputWidget : public Mc3DViewer
{
public:
    InputWidget(QVBoxLayout *vboxLayout);
    void addMesh(QUrl url);
private:
    Qt3DRender::QMesh *mesh;
    QList<QVector3D> vertices;
    QList<uint>indexList;
    void extractMeshData();
private slots:
    void meshStatusChanged(Qt3DRender::QMesh::Status newStatus);
};
