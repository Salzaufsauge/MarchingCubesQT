#ifndef SCALARFIELD_H
#define SCALARFIELD_H

#include <QtCore>
#include "grid.hpp"

class ScalarField
{
public:
    ScalarField();
    void calculateSDF(Grid &grid,const QList<Vector3f> &vertices,const QList<uint> &indices);
};

#endif // SCALARFIELD_H
