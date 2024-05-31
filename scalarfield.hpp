#pragma once

#include <omp.h>
#include <QtCore>
#include <QtConcurrent>
#include <QMutex>
#include <QMutexLocker>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "grid.hpp"

struct Tri{
    Vector3f v0,v1,v2,centroid;
};

class ScalarField
{
public:
    ScalarField();
    void calculateSDF(Grid &grid,const QList<Vector3f> &vertices,const QList<uint> &indices);
    float sdTorus(const Vector3f& p, const Vector2f& t);
private:
    QList<Tri> tris;
    QList<Vector3f> rays;

    void generateTris(const QList<Vector3f> &vertices, const QList<uint> &indices);
    bool isPointInsideMesh(const Vector3f &point);
    bool mollerTromboreIntersect(const Vector3f &point,const Vector3f &ray, const Tri &tri, QList<Vector3f> &prevIntersect);
    bool checkIntersections(const QList<int> &intersections, int rayCount);
    float minPointToTriDist(const Vector3f &point);
    float pointToTriDist(const Vector3f &point, const Tri &tri);

};

