#include "grid.hpp"

Grid::Grid() {}

const QList<QList<QList<Vector3f>>> &Grid::getPoints() const
{
    return points;
}

const QList<QList<QList<float>>> &Grid::getSdf() const
{
    return sdf;
}

void Grid::setSdfAt(int x, int y, int z,const float &newVal)
{
#pragma omp atomic write
    sdf[x][y][z] = newVal;
}

const Vector3i Grid::getRes() const
{
    return res;
}

void Grid::setRes(const Vector3i &newRes)
{
    res = newRes;
}

void Grid::resize(Vector3i newRes)
{
    res = newRes;
    sdf.clear();
    points.clear();
    points.resize(res.x());
    for(auto &y : points){
        y.resize(res.y());
        for(auto &z : y)
            z.resize(res.z());
    }
    sdf.resize(res.x());
    for(auto &y : sdf){
        y.resize(res.y());
        for(auto &z : y)
            z.resize(res.z());
    }
}

void Grid::constructGrid(const uint res,const Vector3f &maxExtendMesh,const Vector3f &minExtendMesh)
{
    maxExtend = maxExtendMesh;
    minExtend = minExtendMesh;
    float lenX = maxExtend.x() - minExtend.x();
    float lenY = maxExtend.y() - minExtend.y();
    float lenZ = maxExtend.z() - minExtend.z();

    float maxLen = std::max({lenX,lenY,lenZ});

    int resX = std::ceil(res * (lenX/maxLen));
    int resY = std::ceil(res * (lenY/maxLen));
    int resZ = std::ceil(res * (lenZ/maxLen));

    resX = std::max(1,resX);
    resY = std::max(1,resY);
    resZ = std::max(1,resZ);

    deltaX = resX > 1 ? lenX / (float)(resX - 1) : 0;
    deltaY = resY > 1 ? lenY / (float)(resY - 1) : 0;
    deltaZ = resZ > 1 ? lenZ / (float)(resZ - 1) : 0;

    resize(Vector3i(resX,resY,resZ));

    for(int i = 0;i < resX; ++i){
        for(int j = 0;j < resY; ++j){
            for(int k = 0;k < resZ; ++k){
                float x = minExtend.x() + deltaX * i;
                float y = minExtend.y() + deltaY * j;
                float z = minExtend.z() + deltaZ * k;
                points[i][j][k] = Vector3f(x,y,z);
            }
        }
    }
}

float Grid::getDeltaX() const
{
    return deltaX;
}

float Grid::getDeltaY() const
{
    return deltaY;
}

float Grid::getDeltaZ() const
{
    return deltaZ;
}




