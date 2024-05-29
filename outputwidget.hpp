#pragma once

#include "mc3dviewer.hpp"

class OutputWidget : public Mc3DViewer
{
public:
    OutputWidget(QVBoxLayout *vboxLayout);
    void buildMesh(const QList<Vector3f> &vertices, const QList<uint> &indices);
    void buildPreviewMesh(const QList<Vector3f> &vertices, const QList<uint> &indices);
    void buildWireframe(const QList<QList<Vector3f>> &cells);
    void clearMesh();
    void clearPreview();
private:
    Mesh *previewMesh;
    QList<Mesh* > wireframe;
};
