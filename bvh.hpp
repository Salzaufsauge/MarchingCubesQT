#pragma once

#include <Eigen/Dense>
#include <QtCore>

struct BVHNode
{
    Eigen::Vector3f aabbMin, aabbMax;
    uint leftChild, rightChild;
    bool isLeaf;
    uint firstPrim, primCount;
};
