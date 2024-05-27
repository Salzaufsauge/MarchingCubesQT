#pragma once

#include <QtCore>
#include "marchingcubes.hpp"

enum class MarchingFlags {
    MARCHING_CUBES = 1,
    DUAL_MARCHING = 2,
    MARCHING_TETRAHEDA = 4,
    SMOOTHING = 8
};

class MarchingController : public QObject
{
    Q_OBJECT
public:
    explicit MarchingController(QObject *parent = nullptr);
    void setSpeed(int newSpeed);
    void calculatePolygons(const Grid &grid,float isolevel, QList<Vector3f> &vertices, QList<uint> &indices, int flags);

private:
    int speed;

signals:
};
