#include "scalarfield.hpp"

ScalarField::ScalarField()
{
}

void ScalarField::calculateSDF(Grid &grid, const QList<Vector3f> &vertices, const QList<uint> &indices)
{
    QMutex mut;
    generateTris(vertices, indices);
    QElapsedTimer start;
    start.start();
    int bad = 0;
    QFuture<void> future = QtConcurrent::map(grid.getPoints(), [&](const Vector3f &point) {
        bool inside = isPointInsideMesh(point);
        if (!inside) {
            QMutexLocker locker(&mut);
            bad++;
        }
    });
    future.waitForFinished();
    qDebug() << start.elapsed() <<bad;
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
    QList<Vector3f> rays = {
        Vector3f(1.f, 0.f, 0.f),
        // Vector3f(0.f, 1.f, 0.f),
        // Vector3f(0.f, 0.f, 1.f),
        Vector3f(-1.f, 0.f, 0.f),
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
