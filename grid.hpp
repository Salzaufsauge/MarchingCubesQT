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
    void setSdfAt(int index,const float &newSdfVal);

    const Vector3i getRes() const;
    void setRes(const Vector3i &newRes);

    void resize(uint newSize = 0);

private:
    QList<Vector3f> points;
    QList<float> sdf;
    Vector3i res;
};
