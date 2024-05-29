#include "marchingcontroller.hpp"

MarchingController::MarchingController(QObject *parent)
    : QObject{parent}
{
    mc = new MarchingCubes(this);
    speed = 0;
    connect(mc,&MarchingCubes::newData,this,&MarchingController::newDataSlot);
}

void MarchingController::setSpeed(int newSpeed)
{
    speed = newSpeed;
}

void MarchingController::calculatePolygons(const Grid &grid, float isolevel, QList<Vector3f> &vertices, QList<uint> &indices, int flags)
{
    running = true;
    if((flags & static_cast<int>(MarchingFlags::MARCHING_CUBES)) == static_cast<int>(MarchingFlags::MARCHING_CUBES))
        mc->mc(grid,isolevel,vertices,indices,speed);
    else if((flags & static_cast<int>(MarchingFlags::DUAL_MARCHING)) == static_cast<int>(MarchingFlags::DUAL_MARCHING)) qDebug() << "not implemented";
    else if((flags & static_cast<int>(MarchingFlags::MARCHING_TETRAHEDA)) == static_cast<int>(MarchingFlags::DUAL_MARCHING)) qDebug() << "not implemented";

    if((flags & static_cast<int>(MarchingFlags::SMOOTHING)) == static_cast<int>(MarchingFlags::SMOOTHING)) qDebug() << "not implemented";
    running = false;
}

bool MarchingController::getRunning() const
{
    return running;
}

void MarchingController::newDataSlot(QList<Vector3f> previewVertices, QList<uint> previewIndices)
{
    emit newData(previewVertices, previewIndices);
}
