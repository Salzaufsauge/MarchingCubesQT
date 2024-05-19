#pragma once

#include <omp.h>
#include <QtCore>
#include <QtConcurrent>
#include <QMutex>
#include <QMutexLocker>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "grid.hpp"

class ScalarField
{
public:
    ScalarField();
    void calculateSDF(Grid &grid,const QList<Vector3f> &vertices,const QList<uint> &indices);
private:
    QList<QList<Vector3f>> tris;

    void generateTris(const QList<Vector3f> &vertices, const QList<uint> &indices);
    bool isPointInsideMesh(const Vector3f &point);
    bool mollerTromboreIntersect(const Vector3f &point,const Vector3f &ray, const QList<Vector3f> &tri, QList<Vector3f> &prevIntersect);
    bool checkIntersections(const QList<int> &intersections, int rayCount);
};

