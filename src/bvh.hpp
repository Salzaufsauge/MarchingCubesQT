#pragma once

#include <Eigen/Dense>

struct BVHNode
{
    Eigen::Vector3f aabbMin, aabbMax;
    uint leftNode, firstTriIdx, triCount;
    bool isLeaf() { return triCount > 0; }
};
