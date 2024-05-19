#include "grid.hpp"

Grid::Grid() {}

const QList<Vector3f> &Grid::getPoints() const
{
    return points;
}

void Grid::appendPoint(const Vector3f &newPoints)
{
    points << newPoints;
}

const QList<float> &Grid::getSdf() const
{
    return sdf;
}

void Grid::appendSdf(const float &newSdf)
{
    sdf << newSdf;
}

const Vector3i Grid::getRes() const
{
    return res;
}

void Grid::setRes(const Vector3i &newRes)
{
    res = newRes;
}

void Grid::resize(uint newSize)
{
    points.clear();
    sdf.clear();
    if(newSize < points.capacity()){
        points.squeeze();
        sdf.squeeze();
    }
    points.reserve(newSize);
    sdf.reserve(newSize);
}



