#pragma once

#include <QtCore>
#include <Eigen/Core>

using namespace Eigen;

class Grid
{
public:
    Grid();
    const QList<Vector3f> &getPoints() const;
    void appendPoint(const Vector3f &newPoint);

    const QList<float> &getSdf() const;
    void appendSdf(const float &newSdf);

    const Vector3i getRes() const;
    void setRes(const Vector3i &newRes);

    void resize(uint newSize = 0);

    QList<bool> getInside() const;
    void setInside(const QList<bool> &newInside);

private:
    QList<Vector3f> points;
    QList<float> sdf;
    Vector3i res;

};
