#include "marchingcontroller.hpp"

MarchingController::MarchingController(QObject *parent)
    : QObject{parent}
{
    speed = 0;
}

void MarchingController::setSpeed(int newSpeed)
{
    speed = newSpeed;
}

void MarchingController::calculatePolygons(const Grid &grid, float isolevel, QList<Vector3f> &vertices, QList<uint> &indices, int flags)
{
    if((flags & static_cast<int>(MarchingFlags::MARCHING_CUBES)) == static_cast<int>(MarchingFlags::MARCHING_CUBES)){
        MarchingCubes mc;
        mc.mc(grid,isolevel,vertices,indices,speed);
    }else if((flags & static_cast<int>(MarchingFlags::DUAL_MARCHING)) == static_cast<int>(MarchingFlags::DUAL_MARCHING)) qDebug() << "not implemented";
    else if((flags & static_cast<int>(MarchingFlags::MARCHING_TETRAHEDA)) == static_cast<int>(MarchingFlags::DUAL_MARCHING)) qDebug() << "not implemented";

    if((flags & static_cast<int>(MarchingFlags::SMOOTHING)) == static_cast<int>(MarchingFlags::SMOOTHING)) qDebug() << "not implemented";
}
