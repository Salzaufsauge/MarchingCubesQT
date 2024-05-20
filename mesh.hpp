#pragma once

#include <Qt3DCore>
#include <Qt3DRender>
#include <Eigen/Dense>

using namespace Eigen;

class Mesh : public Qt3DRender::QMesh
{
    Q_OBJECT
public:
    Mesh(Qt3DCore::QNode *parent = nullptr);
    void extractMeshData();
    const QList<Vector3f> &getVertices() const;
    const QList<uint> &getIndices() const;
    Vector3f getMaxExtend() const;
    Vector3f getMinExtend() const;
    void buildMesh(const QList<Vector3f> &vertices, const QList<uint> &indices);

private:
    QList<Vector3f> vertices;
    QList<uint> indices;
    Vector3f maxExtend;
    Vector3f minExtend;

    void calculateVertexNormals(const QList<Vector3f> &vertices, const QList<uint> &indices, QList<Vector3f> &normals);

signals:
    void dataExtracted();
};
