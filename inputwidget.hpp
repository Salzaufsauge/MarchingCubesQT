#pragma once

#include "mc3dviewer.hpp"

class InputWidget : public Mc3DViewer
{
    Q_OBJECT
public:
    InputWidget(QVBoxLayout *vboxLayout);
    void addMesh(QUrl url);

private slots:
    void meshStatusChanged(Qt3DRender::QMesh::Status newStatus);
};
