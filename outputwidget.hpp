#pragma once

#include "mc3dviewer.hpp"

class OutputWidget : public Mc3DViewer
{
public:
    OutputWidget(QVBoxLayout *vboxLayout);
    void buildMesh(const QList<Vector3f> &vertices, const QList<uint> &indices);
};
