#include "inputwidget.hpp"

InputWidget::InputWidget(QVBoxLayout *vboxLayout)
    :Mc3DViewer(vboxLayout)
{
    mesh = new Qt3DRender::QMesh;
    connect(mesh,&Qt3DRender::QMesh::statusChanged,this,&InputWidget::meshStatusChanged);
}

InputWidget::~InputWidget()
{
    delete mesh;
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
            }
        }else if(attr->attributeType() == Qt3DCore::QAttribute::IndexAttribute){    //extract indices
            QByteArray indexData = attr->buffer()->data();

            const unsigned int* indexList = reinterpret_cast<const unsigned int*>(indexData.constData());
            uint indexCount = indexData.size() / sizeof(unsigned int);
            for(int i = 0; i < indexCount; ++i){
                indices << indexList[i];
            }
        }
    }
    if(!(vertices.isEmpty() && indices.isEmpty()))
        emit dataExtracted();
}

void InputWidget::meshStatusChanged(Qt3DRender::QMesh::Status newStatus)
{
    if(mesh->status() == Qt3DRender::QMesh::Status::Ready){
        extractMeshData();
    }
}
