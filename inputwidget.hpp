#pragma once

#include "mc3dviewer.hpp"

class InputWidget : public Mc3DViewer
{
public:
    InputWidget(QVBoxLayout *vboxLayout);
    void addMesh(QUrl url);
private:
    Qt3DRender::QMesh mesh;

};
