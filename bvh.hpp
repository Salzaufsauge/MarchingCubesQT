#pragma once

#include <Eigen/Dense>

class BVHNode
{
public:
    BVHNode();
    Eigen::Vector3f aabbMin, aabbMax;
    uint leftChild, rightChild;
    bool isLeaf;
    uint firstPrim, primCount;
};
