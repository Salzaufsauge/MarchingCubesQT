#include "scalarfield.hpp"

ScalarField::ScalarField()
{
    rays = QList<Vector3f>{
        Vector3f(1.f, 0.f, 0.f),
        Vector3f(0.f, 1.f, 0.f),
        Vector3f(0.f, 0.f, 1.f),
        Vector3f(-1.f, 0.f, 0.f),
        Vector3f(0.f, -1.f, 0.f),
        Vector3f(0.f, 0.f, -1.f),
        Vector3f(1.f, 1.f, 1.f),
        Vector3f(-1.f, -1.f, -1.f),
        Vector3f(1.f, -0.5f, 0.5f),
        Vector3f(0.5f, 1.f, -0.5f),
        Vector3f(-0.5f, -0.5f, 1.f),
        Vector3f(-1.f, 0.5f, -0.5f),
        Vector3f(0.5f, -1.f, -0.5f),
        Vector3f(-0.5f, -0.5f, -1.f)
    };
}

void ScalarField::calculateSDF(Grid &grid, const QList<Vector3f> &vertices, const QList<uint> &indices)
{
    generateTris(vertices, indices);
    const QList<QList<QList<Vector3f>>> &pointList = grid.getPoints();
    int x = grid.getRes().x();
    int y = grid.getRes().y();
    int z = grid.getRes().z();
#pragma omp parallel for collapse(3)
    for (int i = 0; i < x; ++i){
        for(int j = 0; j < y; ++j){
            for(int k = 0; k < z; ++k){
                bool inside = isPointInsideMesh(pointList[i][j][k]);
                float dist = minPointToTriDist(pointList[i][j][k]);
                grid.setSdfAt(i,j,k, inside ? -dist : dist);
            }
        }
    }
}

void ScalarField::calculateSDFBVH(Grid &grid, const QList<Vector3f> &vertices, const QList<uint> &indices)
{
    generateTris(vertices, indices);
    buildBvh();
    const QList<QList<QList<Vector3f>>> &pointList = grid.getPoints();
    int x = grid.getRes().x();
    int y = grid.getRes().y();
    int z = grid.getRes().z();
#pragma omp parallel for collapse(3)
    for (int i = 0; i < x; ++i){
        for(int j = 0; j < y; ++j){
            for(int k = 0; k < z; ++k){
                bool inside = isPointInsideMeshBVH(pointList[i][j][k]);
                float dist = minPointToTriDistBVH(pointList[i][j][k]);
                grid.setSdfAt(i,j,k, inside ? -dist : dist);
            }
        }
    }
}
//Populates a list of tris
void ScalarField::generateTris(const QList<Vector3f> &vertices, const QList<uint> &indices)
{
    tris.clear();
    tris.squeeze();
    tris.reserve(indices.count()/3);
    for(int i = 0; i < indices.count();i += 3){
        Tri tri;
        tri.v0 = vertices[indices[i]];
        tri.v1 = vertices[indices[i + 1]];
        tri.v2 = vertices[indices[i + 2]];
        tri.centroid = (tri.v0 + tri.v1 + tri.v2) * 0.3333f;
        tris << std::move(tri);
    }
}

bool ScalarField::isPointInsideMesh(const Vector3f &point)
{
    int rayCount = rays.count();
    int trisCount = tris.count();

    QList<int> intersections(rayCount,0);

#pragma omp parallel for
    for(int i = 0; i < rayCount; ++i){
        QList<Vector3f> prevIntersect;
        for(int j = 0; j < trisCount; j++){
            bool r = mollerTromboreIntersect(point,rays[i],tris[j], prevIntersect);
            if(r) {
#pragma omp atomic update
                intersections[i]++;
            }
        }
    }
    return checkIntersections(intersections,rayCount);
}

bool ScalarField::isPointInsideMeshBVH(const Vector3f &point)
{
    int rayCount = rays.count();
    QList<int> intersections(rayCount, 0);
#pragma omp parallel for
    for(int i = 0; i < rayCount; ++i) {
        QList<Vector3f> prevIntersect;
        traverseBVH(point, rays[i], prevIntersect, intersections[i]);
    }
    return checkIntersections(intersections,rayCount);
}

bool ScalarField::mollerTromboreIntersect(const Vector3f &point, const Vector3f &ray, const Tri &tri,  QList<Vector3f> &prevIntersect)
{
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    Vector3f edge1 = tri.v1 - tri.v0;
    Vector3f edge2 = tri.v2 - tri.v0;
    Vector3f rayCross2 = ray.cross(edge2);
    float a = edge1.dot(rayCross2);
    if(std::fabs(a) < epsilon) return false;

    float f = 1.0 / a;
    Vector3f s = point - tri.v0;
    float u = f * s.dot(rayCross2);
    if (u < 0.0f || u > 1.0f) return false;
    Vector3f q = s.cross(edge1);
    float v = f * ray.dot(q);
    if (v < 0.0f || (u + v) > 1.0f) return false;
    float t = f * edge2.dot(q);
    if (t > epsilon) {
        Vector3f uniqueIntersect = point + ray * t;
        float epsilonSquared = epsilon * epsilon;
        for(auto prev : prevIntersect){
            if((prev - uniqueIntersect).squaredNorm() < epsilonSquared) return false;
        }
        prevIntersect << uniqueIntersect;
        return true;
    }
    else return false;
}

bool ScalarField::checkIntersections(const QList<int> &intersections, int rayCount)
{
    bool res = false;
    for(int i = 0; i < rayCount; ++i){

        if(intersections[i] % 2 == 1)
            res = true;
        else if(intersections[i] % 2 != 1 && intersections[i] > 0){
            return false;
        }
    }
    return res;
}

float ScalarField::minPointToTriDist(const Vector3f &point)
{
    float minDistance = std::numeric_limits<float>::infinity();
#pragma omp parallel for
    for(const auto &tri : tris){
        float dist = pointToTriDist(point, tri);
#pragma omp critical
        minDistance = minDistance < dist ? minDistance : dist;
    }
    return std::abs(minDistance);
}

float ScalarField::minPointToTriDistBVH(const Vector3f &point)
{
    float minDistance = std::numeric_limits<float>::infinity();

    QStack<uint> nodesToVisit;
    nodesToVisit.push(rootNodeIdx);

    while (!nodesToVisit.empty()) {
        uint nodeIdx = nodesToVisit.top();
        nodesToVisit.pop();
        BVHNode &node = bvhNode[nodeIdx];

        if (intersectPointAABB(point, node.aabbMin, node.aabbMax)) {
            if (node.isLeaf()) {
#pragma omp parallel for
                for (int i = 0; i < node.triCount; ++i) {
                    const auto &tri = tris[node.firstTriIdx + i];
                    float dist = pointToTriDist(point, tri);
#pragma omp critical
                    minDistance = std::min(minDistance, dist);
                }
            } else {
                nodesToVisit.push(node.leftNode);
                nodesToVisit.push(node.leftNode + 1);
            }
        }
    }

    return std::abs(minDistance);
}

float ScalarField::pointToTriDist(const Vector3f &point, const Tri &tri)
{
    Vector3f edgeBA = tri.v1 - tri.v0;
    Vector3f edgeCA = tri.v2 - tri.v0;
    Vector3f n = edgeBA.cross(edgeCA);
    Vector3f pa = point - tri.v0;
    float distance = abs(pa.dot(n)/ n.norm());
    Vector3f edgeAB = tri.v0 - tri.v1;
    Vector3f edgeAC = tri.v0 - tri.v2;
    Vector3f ap = tri.v0 - point;
    float denom = (edgeAB.cross(edgeAC)).norm();
    float u = (edgeAB.cross(ap)).norm() / denom;
    float v = (edgeAC.cross(ap)).norm() / denom;
    float w = 1 - u - v;
    if (u >= 0 && v >= 0 && w >= 0)
        return distance;
    float minEdgeDist = std::min(ap.cross(edgeBA).norm() / edgeBA.norm(),
                                 std::min((point - tri.v1).cross(tri.v2-tri.v1).norm() / (tri.v2 - tri.v1).norm(),
                                          (point - tri.v2).cross(edgeAC).norm() / edgeAC.norm()));
    return minEdgeDist;
}

void ScalarField::buildBvh()
{
    int trisSize = tris.size();
    bvhNode.resize(trisSize * 2 - 1);
    triIdx.resize(trisSize);

    for (int i = 0; i < trisSize; i++) triIdx[i] = i;
    BVHNode &root = bvhNode[rootNodeIdx];
    root.firstTriIdx = 0;
    root.triCount = trisSize;
    updateNodeBounds(rootNodeIdx);
    subdivide(rootNodeIdx);
}

void ScalarField::updateNodeBounds(uint nodeIdx)
{
    BVHNode &node = bvhNode[nodeIdx];
    node.aabbMax = std::numeric_limits<Vector3f>().min();
    node.aabbMin = std::numeric_limits<Vector3f>().max();
    for(uint first = node.firstTriIdx, i = 0; i < node.triCount; i++){
        Tri& triLeaf = tris[first + i];
        node.aabbMin = node.aabbMin.cwiseMin(triLeaf.v0);
        node.aabbMin = node.aabbMin.cwiseMin(triLeaf.v1);
        node.aabbMin = node.aabbMin.cwiseMin(triLeaf.v2);
        node.aabbMax = node.aabbMax.cwiseMax(triLeaf.v0);
        node.aabbMax = node.aabbMax.cwiseMax(triLeaf.v1);
        node.aabbMax = node.aabbMax.cwiseMax(triLeaf.v2);
    }
}

void ScalarField::subdivide(uint nodeIdx)
{
    BVHNode& node = bvhNode[nodeIdx];
    if (node.triCount <= 2) return;

    Vector3f extent = node.aabbMax - node.aabbMin;
    int axis = 0;
    if (extent.y() > extent.x()) axis = 1;
    if (extent.z() > extent[axis]) axis = 2;
    float splitPos = node.aabbMin[axis] + extent[axis] * 0.5f;

    int i = node.firstTriIdx;
    int j = i + node.triCount - 1;
    while (i <= j)
    {
        if (tris[triIdx[i]].centroid[axis] < splitPos)
            i++;
        else
            std::swap( triIdx[i], triIdx[j--] );
    }

    int leftCount = i - node.firstTriIdx;
    if (leftCount == 0 || leftCount == node.triCount) return;

    int leftChildIdx = nodesUsed++;
    int rightChildIdx = nodesUsed++;
    bvhNode[leftChildIdx].firstTriIdx = node.firstTriIdx;
    bvhNode[leftChildIdx].triCount = leftCount;
    bvhNode[rightChildIdx].firstTriIdx = i;
    bvhNode[rightChildIdx].triCount = node.triCount - leftCount;
    node.leftNode = leftChildIdx;
    node.triCount = 0;
    updateNodeBounds( leftChildIdx );
    updateNodeBounds( rightChildIdx );

    subdivide( leftChildIdx );
    subdivide( rightChildIdx );
}

void ScalarField::traverseBVH(const Vector3f &point, const Vector3f &ray, QList<Vector3f> &prevIntersect, int &intersectionCount)
{
    QStack<uint> nodesToVisit;
    nodesToVisit.push(rootNodeIdx);

    while (!nodesToVisit.empty()) {
        uint nodeIdx = nodesToVisit.top();
        nodesToVisit.pop();
        BVHNode &node = bvhNode[nodeIdx];

        if (!intersectRayAABB(point, ray, node.aabbMin, node.aabbMax)) {
            continue;
        }

        if (node.isLeaf()) {
#pragma omp parallel for
            for (int i = 0; i < node.triCount; ++i) {
                if (mollerTromboreIntersect(point, ray, tris[node.firstTriIdx + i], prevIntersect)) {
#pragma omp atomic update
                    intersectionCount++;
                }
            }
        } else {
            nodesToVisit.push(node.leftNode);
            nodesToVisit.push(node.leftNode + 1);
        }
    }
}

bool ScalarField::intersectRayAABB(const Vector3f &point, const Vector3f &ray, const Vector3f &aabbMin, const Vector3f &aabbMax)
{
    float tmin = (aabbMin.x() - point.x()) / ray.x();
    float tmax = (aabbMax.x() - point.x()) / ray.x();

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (aabbMin.y() - point.y()) / ray.y();
    float tymax = (aabbMax.y() - point.y()) / ray.y();

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax)) return false;

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    float tzmin = (aabbMin.z() - point.z()) / ray.z();
    float tzmax = (aabbMax.z() - point.z()) / ray.z();

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax)) return false;

    return true;
}

bool ScalarField::intersectPointAABB(const Vector3f &point, const Vector3f &aabbMin, const Vector3f &aabbMax)
{
    return (point.x() >= aabbMin.x() && point.x() <= aabbMax.x()) &&
           (point.y() >= aabbMin.y() && point.y() <= aabbMax.y()) &&
           (point.z() >= aabbMin.z() && point.z() <= aabbMax.z());
}
