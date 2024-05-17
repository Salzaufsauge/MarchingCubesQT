#pragma once

#include "mc3dviewer.hpp"

class InputWidget : public Mc3DViewer
{
    Q_OBJECT
public:
    InputWidget(QVBoxLayout *vboxLayout);
    void addMesh(QUrl url);
private:
    Qt3DRender::QMesh *mesh;
    QList<QVector3D> vertices;
    QList<uint>indices;
    QList<QVector3D> grid;
    QVector3D maxExtend;
    QVector3D minExtend;

    void extractMeshData();
    void constructGrid(const unsigned int res);
private slots:
    void meshStatusChanged(Qt3DRender::QMesh::Status newStatus);
signals:
    void dataExtracted();
};
