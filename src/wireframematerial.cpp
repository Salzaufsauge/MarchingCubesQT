#include "wireframematerial.hpp"

WireframeMaterial::WireframeMaterial(Qt3DCore::QNode *parent)
    :Qt3DRender::QMaterial(parent)
{
    diffuse = QColor(Qt::red);
    ambient = QColor(0,0,0);
    specular = QColor(255,255,255);
    shininess = 150.f;

    addParameter(new Qt3DRender::QParameter(QStringLiteral("ka"), ambient));
    addParameter(new Qt3DRender::QParameter(QStringLiteral("kd"), diffuse));
    addParameter(new Qt3DRender::QParameter(QStringLiteral("ksp"), specular));
    addParameter(new Qt3DRender::QParameter(QStringLiteral("shininess"), shininess));
    addParameter(new Qt3DRender::QParameter(QStringLiteral("line.width"), 0.8f));
    addParameter(new Qt3DRender::QParameter(QStringLiteral("line.color"), QColor(0,0,0)));

    Qt3DRender::QEffect *effect = new Qt3DRender::QEffect();
    effect->addParameter(new Qt3DRender::QParameter(QStringLiteral("ka"),ambient));
    effect->addParameter(new Qt3DRender::QParameter(QStringLiteral("kd"),diffuse));
    effect->addParameter(new Qt3DRender::QParameter(QStringLiteral("ks"),specular));
    effect->addParameter(new Qt3DRender::QParameter(QStringLiteral("shininess"),shininess));


    Qt3DRender::QTechnique *technique = new Qt3DRender::QTechnique();
    technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
    technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::CoreProfile);
    technique->graphicsApiFilter()->setMajorVersion(3);
    technique->graphicsApiFilter()->setMinorVersion(1);

    technique->addParameter(new Qt3DRender::QParameter(QStringLiteral("light.position"), QVector4D(0.0f, 0.0f, 0.0f, 1.0f)));
    technique->addParameter(new Qt3DRender::QParameter(QStringLiteral("light.intensity"), QVector3D(1.0f, 1.0f, 1.0f)));
    technique->addParameter(new Qt3DRender::QParameter(QStringLiteral("line.width"), 0.5f));
    technique->addParameter(new Qt3DRender::QParameter(QStringLiteral("line.color"), QColor(255, 255, 255)));

    Qt3DRender::QRenderPass *renderPass = new Qt3DRender::QRenderPass();
    Qt3DRender::QFilterKey *filterKey = new Qt3DRender::QFilterKey();
    filterKey->setName(QStringLiteral("renderingStyle"));
    filterKey->setValue(QStringLiteral("forward"));

    Qt3DRender::QShaderProgram *shader = new Qt3DRender::QShaderProgram();
    shader->setVertexShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shaders/wireframe.vert"))));
    shader->setFragmentShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shaders/wireframe.frag"))));
    shader->setGeometryShaderCode(Qt3DRender::QShaderProgram::loadSource(QUrl(QStringLiteral("qrc:/shaders/wireframe.geom"))));

    renderPass->setShaderProgram(shader);
    technique->addFilterKey(filterKey);
    technique->addRenderPass(renderPass);
    effect->addTechnique(technique);

    setEffect(effect);
}
