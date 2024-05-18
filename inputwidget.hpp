#pragma once

#include "mc3dviewer.hpp"
#include "grid.hpp"

class InputWidget : public Mc3DViewer
{
    Q_OBJECT
public:
    InputWidget(QVBoxLayout *vboxLayout);
    void addMesh(QUrl url);
    void constructGrid(const uint res);
    Grid &getGrid();
    const QList<Vector3f> &getVertices() const;
    const QList<uint> &getIndices() const;

private:
    Qt3DRender::QMesh *mesh;
    QList<Vector3f> vertices;
    QList<uint> indices;
    Grid grid;
    Vector3f maxExtend;
    Vector3f minExtend;

    void extractMeshData();

private slots:
    void meshStatusChanged(Qt3DRender::QMesh::Status newStatus);
signals:
    void dataExtracted();
};
