#include "inputwidget.hpp"

InputWidget::InputWidget(QVBoxLayout *vboxLayout)
    :Mc3DViewer(vboxLayout)
{
    mesh = new Qt3DRender::QMesh(objectEntity);
    connect(mesh,&Qt3DRender::QMesh::statusChanged,this,&InputWidget::meshStatusChanged);
}

void InputWidget::addMesh(QUrl url)
{
    mesh->setSource(url);
    objectEntity->addComponent(mesh);
}

void InputWidget::extractMeshData()
{
    vertices.clear();
    indices.clear();
    auto geometry = mesh->geometry();
    minExtend = QVector3D(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max());
    maxExtend = QVector3D(std::numeric_limits<float>::min(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min());
    const auto attrList = geometry->attributes();
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
                vertices << QVector3D(fpos[0],fpos[1],fpos[2]);
                minExtend.setX(std::min(minExtend.x(),fpos[0]));
                minExtend.setY(std::min(minExtend.y(),fpos[1]));
                minExtend.setZ(std::min(minExtend.z(),fpos[2]));
                maxExtend.setX(std::max(maxExtend.x(),fpos[0]));
                maxExtend.setY(std::max(maxExtend.y(),fpos[1]));
                maxExtend.setZ(std::max(maxExtend.z(),fpos[2]));
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

void InputWidget::constructGrid(unsigned int res)
{
    if(!(mesh->status() == Qt3DRender::QMesh::Status::Ready)){
        qDebug() << "mesh not ready";
    }
    grid.clear();
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

    float deltaX;
    float deltaY;
    float deltaZ;

    deltaX = resX > 1 ? lenX / (float)(resX - 1) : 0;
    deltaY = resY > 1 ? lenY / (float)(resY - 1) : 0;
    deltaZ = resZ > 1 ? lenZ / (float)(resZ - 1) : 0;

    grid.reserve(resX*resY*resZ);

    for(int i = 0;i < resX; ++i){
        for(int j = 0;j < resY; ++j){
            for(int k = 0;k < resZ; ++k){
                float x = minExtend.x() + deltaX * i;
                float y = minExtend.y() + deltaY * j;
                float z = minExtend.z() + deltaZ * k;
                grid << QVector3D(x,y,z);
            }
        }
    }
}

const QList<QVector3D> &InputWidget::getGrid() const
{
    return grid;
}

const QList<QVector3D> &InputWidget::getVertices() const
{
    return vertices;
}

const QList<uint> &InputWidget::getIndices() const
{
    return indices;
}

void InputWidget::meshStatusChanged(Qt3DRender::QMesh::Status newStatus)
{
    if(newStatus == Qt3DRender::QMesh::Status::Ready){
        extractMeshData();
    }
}
