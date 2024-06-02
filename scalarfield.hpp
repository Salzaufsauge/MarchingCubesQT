#pragma once

#include <omp.h>
#include <QtCore>
#include <QtConcurrent>
#include <QMutex>
#include <QMutexLocker>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "bvh.hpp"
#include "grid.hpp"

struct Tri{
    Eigen::Vector3f v0,v1,v2,centroid;
};

class ScalarField
{
public:
    ScalarField();
    void calculateSDF(Grid &grid,const QList<Vector3f> &vertices,const QList<uint> &indices);
    void calculateSDFBVH(Grid &grid,const QList<Vector3f> &vertices,const QList<uint> &indices);
private:
    QList<Tri> tris;
    QList<uint> triIdx;
    QList<Vector3f> rays;
    QList<BVHNode> bvhNode;
    uint rootNodeIdx = 0, nodesUsed = 1;

    void generateTris(const QList<Vector3f> &vertices, const QList<uint> &indices);
    bool isPointInsideMesh(const Vector3f &point);
    bool isPointInsideMeshBVH(const Vector3f &point);
    bool mollerTromboreIntersect(const Vector3f &point,const Vector3f &ray, const Tri &tri, QList<Vector3f> &prevIntersect);
    bool checkIntersections(const QList<int> &intersections, int rayCount);
    float minPointToTriDist(const Vector3f &point);
    float minPointToTriDistBVH(const Vector3f &point);
    float pointToTriDist(const Vector3f &point, const Tri &tri);
    void buildBvh();
    void updateNodeBounds(uint nodeIdx);
    void subdivide(uint nodeIdx);
    void traverseBVH(const Vector3f &point, const Vector3f &ray, QList<Vector3f> &prevIntersect, int &intersectionCount);
    bool intersectRayAABB(const Vector3f &point, const Vector3f &ray, const Vector3f &aabbMin, const Vector3f &aabbMax);
    bool intersectPointAABB(const Vector3f &point, const Vector3f &aabbMin, const Vector3f &aabbMax);
};

