#pragma once

#include "mc3dviewer.hpp"

class InputWidget : public Mc3DViewer
{
    Q_OBJECT
public:
    InputWidget(QVBoxLayout *vboxLayout);
    ~InputWidget();
    void addMesh(QUrl url);
private:
    Qt3DRender::QMesh *mesh;
    QList<QVector3D> vertices;
    QList<uint>indices;
    void extractMeshData();
private slots:
    void meshStatusChanged(Qt3DRender::QMesh::Status newStatus);
signals:
    void dataExtracted();
};
