#include "scalarfield.hpp"

ScalarField::ScalarField()
{
    rays = QList<Vector3f>{
        Vector3f(1.f, 0.f, 0.f),
        // Vector3f(0.f, 1.f, 0.f),
        // Vector3f(0.f, 0.f, 1.f),
        // Vector3f(-1.f, 0.f, 0.f),
        // Vector3f(0.f, -1.f, 0.f),
        // Vector3f(0.f, 0.f, -1.f),
        // Vector3f(1.f, 1.f, 1.f),
        // Vector3f(-1.f, -1.f, -1.f),
        // Vector3f(1.f, -0.5f, 0.5f),
        // Vector3f(0.5f, 1.f, -0.5f),
        // Vector3f(-0.5f, -0.5f, 1.f),
        // Vector3f(-1.f, 0.5f, -0.5f),
        // Vector3f(0.5f, -1.f, -0.5f),
        // Vector3f(-0.5f, -0.5f, -1.f)
    };
}

void ScalarField::calculateSDF(Grid &grid, const QList<Vector3f> &vertices, const QList<uint> &indices)
{
    generateTris(vertices, indices);
    const QList<QList<QList<Vector3f>>> &pointList = grid.getPoints();
    int x = grid.getRes().x();
    int y = grid.getRes().y();
    int z = grid.getRes().z();
#pragma omp parallel for
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
//Populates a list of tris
void ScalarField::generateTris(const QList<Vector3f> &vertices, const QList<uint> &indices)
{
    tris.clear();
    tris.squeeze();
    tris.reserve(indices.count()/3);
    for(int i = 0; i < indices.count();i += 3){
        QList<Vector3f> tri;
        tri.reserve(3);
        tri << Vector3f(vertices[indices[i]])
            << Vector3f(vertices[indices[i + 1]])
            << Vector3f(vertices[indices[i + 2]]);
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

bool ScalarField::mollerTromboreIntersect(const Vector3f &point, const Vector3f &ray, const QList<Vector3f> &tri,  QList<Vector3f> &prevIntersect)
{
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    if(tri.size() != 3) return false;
    Vector3f edge1 = tri[1] - tri[0];
    Vector3f edge2 = tri[2] - tri[0];
    Vector3f rayCross2 = ray.cross(edge2);
    float a = edge1.dot(rayCross2);
    if(std::fabs(a) < epsilon) return false;

    float f = 1.0 / a;
    Vector3f s = point - tri[0];
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
    for(const auto &tri : tris){
        float dist = pointToTriDist(point, tri);
        minDistance = minDistance < dist ? minDistance : dist;
    }
    return std::abs(minDistance);
}

float ScalarField::pointToTriDist(const Vector3f &point, const QList<Vector3f> &tri)
{
    Vector3f edgeBA = tri[1] - tri[0];
    Vector3f edgeCA = tri[2] - tri[0];
    Vector3f n = edgeBA.cross(edgeCA);
    Vector3f pa = point - tri[0];
    float distance = abs(pa.dot(n)/ n.norm());
    Vector3f edgeAB = tri[0] - tri[1];
    Vector3f edgeAC = tri[0] - tri[2];
    Vector3f ap = tri[0] - point;
    float denom = (edgeAB.cross(edgeAC)).norm();
    float u = (edgeAB.cross(ap)).norm() / denom;
    float v = (edgeAC.cross(ap)).norm() / denom;
    float w = 1 - u - v;
    if (u >= 0 && v >= 0 && w >= 0)
        return distance;
    float minEdgeDist = std::min(ap.cross(edgeBA).norm() / edgeBA.norm(),
                                 std::min((point - tri[1]).cross(tri[2]-tri[1]).norm() / (tri[2] - tri[1]).norm(),
                                          (point - tri[2]).cross(edgeAC).norm() / edgeAC.norm()));
    return minEdgeDist;
}

float ScalarField::sdTorus(const Vector3f& p, const Vector2f& t) {
    Vector2f q(p.head<2>().norm() - t.x(), p.y());
    return q.norm() - t.y();
}
