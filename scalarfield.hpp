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
    QList<Vector3f> rays = {
        Vector3f(1.f, 0.f, 0.f),
        // Vector3f(0.f, 1.f, 0.f),
        // Vector3f(0.f, 0.f, 1.f),
        Vector3f(-1.f, 0.f, 0.f),
        // Vector3f(0.f, -1.f, 0.f),
        // Vector3f(0.f, 0.f, -1.f),
        // Vector3f(1.f, 1.f, 1.f),
        // Vector3f(-1.f, -1.f, -1.f),
        // Vector3f(1.f, -0.5f, 0.5f),
        // Vector3f(0.5f, 1.f, -0.5f),
        // Vector3f(-0.5f, -0.5f, 1.f),
        // Vector3f(-1.f, 0.5f, -0.5f),
        // Vector3f(0.5f, -1.f, -0.5f),
        // Vector3f(-0.5f, -0.5f, -1.f)
    };

    void generateTris(const QList<Vector3f> &vertices, const QList<uint> &indices);
    bool isPointInsideMesh(const Vector3f &point);
    bool mollerTromboreIntersect(const Vector3f &point,const Vector3f &ray, const QList<Vector3f> &tri, QList<Vector3f> &prevIntersect);
    bool checkIntersections(const QList<int> &intersections, int rayCount);
    float minPointToTriDist(const Vector3f &point);
    float pointToTriDist(const Vector3f &point, const QList<Vector3f> &tri);
};

