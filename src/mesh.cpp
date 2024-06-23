#include "mesh.hpp"

Mesh::Mesh(Qt3DCore::QNode *parent)
    :Qt3DRender::QMesh(parent)
{
}

void Mesh::extractMeshData()
{
    vertices.clear();
    vertices.squeeze();
    indices.clear();
    indices.squeeze();
    minExtend = Vector3f(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max());
    maxExtend = Vector3f(std::numeric_limits<float>::min(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min());
    const auto attrList = geometry()->attributes();
    uint byteStride = 0;
    uint byteOffset = 0;
    for(Qt3DCore::QAttribute* attr : attrList) {
        if(attr->name() == Qt3DCore::QAttribute::defaultPositionAttributeName()) {  //extract vertices
            QByteArray vertexData = attr->buffer()->data();
            byteStride = attr->byteStride() == 0 ? (3 * sizeof(float)) : attr->byteStride();
            byteOffset = attr->byteOffset();

            uint vertexCount = vertexData.size() / byteStride;
            vertices.reserve(vertexCount);
            for(int i = 0; i < vertexCount; ++i) {
                int index = byteOffset + i * byteStride;
                //x, y, z pos
                const float* fpos = reinterpret_cast<const float*>(vertexData.constData() + index);
                vertices << Vector3f(fpos[0],fpos[1],fpos[2]);
                minExtend.x() = std::min(minExtend.x(),fpos[0]);
                minExtend.y() = std::min(minExtend.y(),fpos[1]);
                minExtend.z() = std::min(minExtend.z(),fpos[2]);
                maxExtend.x() = std::max(maxExtend.x(),fpos[0]);
                maxExtend.y() = std::max(maxExtend.y(),fpos[1]);
                maxExtend.z() = std::max(maxExtend.z(),fpos[2]);
            }
        }else if(attr->attributeType() == Qt3DCore::QAttribute::IndexAttribute){    //extract indices
            QByteArray indexData = attr->buffer()->data();
            switch(attr->vertexBaseType()){
            case Qt3DCore::QAttribute::VertexBaseType::UnsignedInt:{
                const unsigned int* indexList = reinterpret_cast<const unsigned int*>(indexData.constData());
                uint indexCount = indexData.size() / sizeof(unsigned int);
                for(int i = 0; i < indexCount; ++i){
                    indices << indexList[i];
                }
                break;
            }
            case Qt3DCore::QAttribute::VertexBaseType::UnsignedShort:{
                const unsigned short* indexList = reinterpret_cast<const unsigned short*>(indexData.constData());
                unsigned short indexCount = indexData.size() / sizeof(unsigned short);
                for(int i = 0; i < indexCount; ++i){
                    indices <<  static_cast<unsigned int>(indexList[i]);
                }
                break;
            }
            default:
                qDebug() << QString("unsupported type");
                break;
            }
        }
    }
    if(!(vertices.isEmpty() && indices.isEmpty()))
        emit dataExtracted();
}

const QList<Vector3f> &Mesh::getVertices() const
{
    return vertices;
}

const QList<uint> &Mesh::getIndices() const
{
    return indices;
}

Vector3f Mesh::getMaxExtend() const
{
    return maxExtend;
}

Vector3f Mesh::getMinExtend() const
{
    return minExtend;
}

void Mesh::buildMesh(const QList<Vector3f> &vertices, const QList<uint> &indices, bool withNormals)
{
    this->vertices = vertices;
    this->indices = indices;

    Qt3DCore::QGeometry *geom = new Qt3DCore::QGeometry(this);
    Qt3DCore::QBuffer *vertexDataBuffer = new Qt3DCore::QBuffer(geom);
    Qt3DCore::QBuffer *indexDataBuffer = new Qt3DCore::QBuffer(geom);

    Qt3DCore::QAttribute *positionAttribute = new Qt3DCore::QAttribute();
    Qt3DCore::QAttribute *indexAttribute = new Qt3DCore::QAttribute();

    QByteArray vertexData;
    QByteArray indexData;

    vertexData.resize(vertices.size() * 3 * sizeof(float));
    float *vertexDataPtr = reinterpret_cast<float*>(vertexData.data());
    for(const Vector3f &v : vertices){
        *vertexDataPtr++ = v.x();
        *vertexDataPtr++ = v.y();
        *vertexDataPtr++ = v.z();
    }

    indexData.resize(indices.size() * sizeof(uint));
    uint *indexDataPtr = reinterpret_cast<uint*>(indexData.data());
    for(uint i : indices)
        *indexDataPtr++ = i;

    vertexDataBuffer->setData(vertexData);
    indexDataBuffer->setData(indexData);

    positionAttribute->setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(vertexDataBuffer);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(vertices.size());

    indexAttribute->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
    indexAttribute->setVertexBaseType(Qt3DCore::QAttribute::UnsignedInt);
    indexAttribute->setBuffer(indexDataBuffer);
    indexAttribute->setCount(indices.size());

    geom->addAttribute(positionAttribute);
    geom->addAttribute(indexAttribute);

    if(withNormals){
        normals.clear();
        normals.resize(vertices.size(), Vector3f(0.0f, 0.0f, 0.0f));
        calculateVertexNormals(vertices,indices,normals);
        Qt3DCore::QBuffer *normDataBuffer = new Qt3DCore::QBuffer(geom);
        Qt3DCore::QAttribute *normAttribute = new Qt3DCore::QAttribute();
        QByteArray normData;
        normData.resize(normals.size() * 3 * sizeof(float));
        float *normDataPtr = reinterpret_cast<float*>(normData.data());
        for(const Vector3f &v : normals){
            *normDataPtr++ = v.x();
            *normDataPtr++ = v.y();
            *normDataPtr++ = v.z();
        }
        normDataBuffer->setData(normData);
        normAttribute->setName(Qt3DCore::QAttribute::defaultNormalAttributeName());
        normAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
        normAttribute->setVertexSize(3);
        normAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
        normAttribute->setBuffer(normDataBuffer);
        normAttribute->setByteStride(3 * sizeof(float));
        normAttribute->setCount(normals.size());
        geom->addAttribute(normAttribute);
    }
    setGeometry(geom);
}

const QList<Vector3f> &Mesh::getNormals() const
{
    return normals;
}

void Mesh::calculateVertexNormals(const QList<Vector3f> &vertices, const QList<uint> &indices, QList<Vector3f> &normals)
{
#pragma omp parallel for
    for (int i = 0; i < indices.size(); i += 3) {

        uint idx1 = indices[i];
        uint idx2 = indices[i + 1];
        uint idx3 = indices[i + 2];

        Vector3f v1 = vertices[idx1];
        Vector3f v2 = vertices[idx2];
        Vector3f v3 = vertices[idx3];
        Vector3f faceNormal = (v2 - v1).cross(v3 - v1).normalized();
#pragma omp critical(norm)
        {
            normals[idx1] += faceNormal;
            normals[idx2] += faceNormal;
            normals[idx3] += faceNormal;
        }
    }
}
