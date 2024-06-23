#pragma once

#include <QtCore>
#include <Eigen/Core>

using namespace Eigen;

class Grid
{
public:
    Grid();
    const QList<QList<QList<Vector3f>>> &getPoints() const;
    void appendPoint(const Vector3f &newPoint);

    const QList<QList<QList<float>>> &getSdf() const;
    void setSdfAt(int x, int y, int z,const float &newSdfVal);

    const Vector3i getRes() const;
    void setRes(const Vector3i &newRes);

    void resize(Vector3i newRes);

    void constructGrid(const uint res,const Vector3f &maxExtendMesh,const Vector3f &minExtendMesh);

    float getDeltaX() const;

    float getDeltaY() const;

    float getDeltaZ() const;

private:
    QList<QList<QList<Vector3f>>> points;
    QList<QList<QList<float>>> sdf;
    Vector3f minExtend;
    Vector3f maxExtend;
    Vector3i res;
    float deltaX;
    float deltaY;
    float deltaZ;
};
