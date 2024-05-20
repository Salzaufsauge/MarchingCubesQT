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

private:
    Grid grid;

private slots:
    void meshStatusChanged(Qt3DRender::QMesh::Status newStatus);
};
