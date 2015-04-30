#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    limbNum = 0;

    setFocusPolicy(Qt::StrongFocus);
    body = Body();
    forward_leg = Leg_Forward::LEFT;
}

GLWidget::~GLWidget()
{
    makeCurrent();
    vbo.destroy();
    delete program;
    doneCurrent();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    // Enable back face culling
    glEnable(GL_CULL_FACE);

    clearColor.setRgbF(0.1, 0.1, 0.1, 1.0);

    vao.create();
    vao.bind();
    initShaders();
    loadBody();
}

void GLWidget::initShaders()
{
#define PROGRAM_VERTEX_ATTRIBUTE 0

    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    const char *vsrc =
            "#version 330\n"
            "layout (location = 0) in vec4 vertex;\n"
            "void main(void)\n"
            "{\n"
            "   gl_Position = vertex;\n"
            "}\n";
    vshader->compileSourceCode(vsrc);

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    const char *fsrc =
            "#version 330\n"
            "uniform vec3 m_color;\n"
            "out vec3 color;\n"
            "void main(void)\n"
            "{\n"
            "   color = m_color;\n"
            "}\n";
    fshader->compileSourceCode(fsrc);

    program = new QOpenGLShaderProgram;
    program->addShader(vshader);
    program->addShader(fshader);
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->link();
    program->bind();
}

void GLWidget::loadBody()
{
    //must specify vertices in a cyclic order to obtain convex polygon
    static const GLfloat g_vertex_buffer_data[6][3]= {
        {-1.0f, -1.0f, 0.0f}, //bot left v1
        {1.0f,-1.0f, 0.0f}, //bot right v2
        {1.0f,1.0f,0.0f}, //top right v3
        {-1.0f,1.0f, 0.0f} //top left v4
    };
    float coords[4][3];
    Limb* l;
    moveLimb(g_vertex_buffer_data, coords, 0.0f, 0.0f, 0.0f, 0.10f);
    addLimb(coords);
    addLimbAtJoint(*body.getLimbAt(0), 0.0f, -0.1f, 0.075f, 0.30f); //thigh 1
    addLimbAtJoint(*body.getLimbAt(0), 0.0f, -0.1f, 0.075f, 0.30f); //thigh 2
    l = addLimbAtJoint(*body.getLimbAt(1), 0.0f, -0.40f, 0.075f, 0.20f); //shin 1
    l->setMaxRot(0.0f);
    l->setMinRot(-M_PI/4.0f);
    l->setRotationDirection(Limb::Rotation_Direction::CW);
    l->rotateLimbAboutJoint(M_PI/4.0f);
    l->setRotationDirection(Limb::Rotation_Direction::CCW);
    l = addLimbAtJoint(*body.getLimbAt(2), 0.0f, -0.40f, 0.075f, 0.20f); //shin 2
    l->setMaxRot(0.0f);
    l->setMinRot(-M_PI/4.0f);
    l->setRotationDirection(Limb::Rotation_Direction::CW);
    body.getLimbAt(2)->setRotationDirection(Limb::Rotation_Direction::CW);
    QVector<float> vertices = body.getLimbVertices();
    vbo.create();
    vbo.bind();
    vbo.allocate(vertices.constData(), body.getVertexCount() * sizeof(float));
    //vbo.allocate(g_vertex_buffer_data, sizeof(g_vertex_buffer_data));
}

void GLWidget::animate() {
    if (forward_leg == Leg_Forward::LEFT &&
            body.getLimbAt(1)->getCurrRot() >= body.getLimbAt(1)->getMaxRot()) {
        body.getLimbAt(1)->setRotationDirection(Limb::Rotation_Direction::CW);
        body.getLimbAt(2)->setRotationDirection(Limb::Rotation_Direction::CCW);
        forward_leg = Leg_Forward::RIGHT;
    } else if (forward_leg == Leg_Forward::RIGHT &&
               body.getLimbAt(2)->getCurrRot() >= body.getLimbAt(2)->getMaxRot()) {
           body.getLimbAt(1)->setRotationDirection(Limb::Rotation_Direction::CCW);
           body.getLimbAt(2)->setRotationDirection(Limb::Rotation_Direction::CW);
           forward_leg = Leg_Forward::LEFT;
    }
    if (forward_leg == Leg_Forward::LEFT) {
        legForward(body.getLimbAt(1));
        legBackward(body.getLimbAt(2));
    } else {
        legForward(body.getLimbAt(2));
        legBackward(body.getLimbAt(1));
    }
    /*
    body.getLimbAt(1)->rotateLimbAboutJoint(M_PI/540.0f);
    body.getLimbAt(2)->rotateLimbAboutJoint(M_PI/540.0f);

    */
    QVector<float> vertices = body.getLimbVertices();

    vbo.allocate(vertices.constData(), body.getVertexCount() * sizeof(float));
    update();
}

void GLWidget::legForward(Limb* leg) {

    leg->rotateLimbAboutJoint(M_PI/810.0f);

    if (leg->getCurrRot() >= 0.0f) {
        leg->getChild()->setRotationDirection(Limb::Rotation_Direction::CCW);
        straightenKnee(leg->getChild());
    }
    if (leg->getCurrRot() <= 0.0f) {
        leg->getChild()->setRotationDirection(Limb::Rotation_Direction::CW);
        bendKnee(leg->getChild());
    }

}

void GLWidget::legBackward(Limb* leg) {
    leg->rotateLimbAboutJoint(M_PI/810.f);
    /*
    if (leg->getCurrRot() <= 0.0f) {
        bendKnee(leg->getChild());
    }*/
}

void GLWidget::bendKnee(Limb* shin) {
    shin->rotateLimbAboutJoint(M_PI/270.0f);
}

void GLWidget::straightenKnee(Limb* shin) {
    shin->rotateLimbAboutJoint(M_PI/270.0f);
}

Limb* GLWidget::addLimb(float coords[4][3]) {
    Limb* l = new Limb();
    l->setV1(coords[0][0], coords[0][1]);
    l->setV2(coords[1][0], coords[1][1]);
    l->setV3(coords[2][0], coords[2][1]);
    l->setV4(coords[3][0], coords[3][1]);
    body.addLimb(l);
    limbNum++;
    return l;
}

Limb* GLWidget::addLimbAtJoint(Limb &parent, float jointX, float jointY, float scaleX, float scaleY) {
    float scaleDiff;
    Limb::Joint_Side js = parent.jointSide(jointX, jointY);
    if (js != Limb::Joint_Side::NON_JOINT) {
        Limb* l = new Limb();
        parent.setChild(l);
        l->setParent(&parent);
        l->setJoint(jointX, jointY);
        if (js == Limb::Joint_Side::LEFT) {
            scaleDiff = (parent.getV4().y() - parent.getV1().y()) - scaleY;
            l->setV2(parent.getV1() + QVector2D(0.0f, scaleDiff/2.0f));
            l->setV3(parent.getV4() - QVector2D(0.0f, scaleDiff/2.0f));
            l->setV1((parent.getV1() + QVector2D(0.0f, scaleDiff/2.0f)) - QVector2D(scaleX,0));
            l->setV4((parent.getV4() - QVector2D(0.0f, scaleDiff/2.0f)) - QVector2D(scaleX,0));
        } else if (js ==  Limb::Joint_Side::RIGHT) {
            scaleDiff = (parent.getV3().y() - parent.getV2().y()) - scaleY;
            l->setV1(parent.getV2() + QVector2D(0.0f, scaleDiff/2.0f));
            l->setV4(parent.getV3() - QVector2D(0.0f, scaleDiff/2.0f));
            l->setV2((parent.getV2() + QVector2D(0.0f, scaleDiff/2.0f)) + QVector2D(scaleX,0));
            l->setV3((parent.getV3() - QVector2D(0.0f, scaleDiff/2.0f)) + QVector2D(scaleX,0));
        } else if (js == Limb::Joint_Side::BOTTOM) {
            scaleDiff = (parent.getV2().x() - parent.getV1().x()) - scaleX;
            l->setV4(parent.getV1() + QVector2D(scaleDiff/2.0f, 0.0f));
            l->setV3(parent.getV2() - QVector2D(scaleDiff/2.0f, 0.0f));
            l->setV1((parent.getV1() + QVector2D(scaleDiff/2.0f, 0.0f)) - QVector2D(0,scaleY));
            l->setV2((parent.getV2() - QVector2D(scaleDiff/2.0f, 0.0f)) - QVector2D(0,scaleY));
        } else {
            scaleDiff = (parent.getV3().x() - parent.getV4().x()) - scaleX;
            l->setV1(parent.getV4() + QVector2D(scaleDiff/2.0f, 0.0f));
            l->setV2(parent.getV3() - QVector2D(scaleDiff/2.0f, 0.0f));
            l->setV4((parent.getV4() + QVector2D(scaleDiff/2.0f, 0.0f)) + QVector2D(0,scaleY));
            l->setV3((parent.getV3() - QVector2D(scaleDiff/2.0f, 0.0f)) + QVector2D(0,scaleY));
        }
        body.addLimb(l);
        limbNum++;
        return l;
    } else {
        return NULL;
    }
}

void GLWidget::moveLimb(const float coords[4][3], float (&returnCoords)[4][3], float x, float y, float z, float scale){
    for (int i = 0; i < 4; i++) {
        returnCoords[i][0] = (coords[i][0]+x) * scale;
        returnCoords[i][1] = (coords[i][1]+y) * scale;
        returnCoords[i][2] = (coords[i][2]+z) * scale;
    }
}

void GLWidget::paintGL()
{
    //background
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 0);

    QVector3D color[5] = {
        QVector3D(0.3f,0.0f,0.0f),
        QVector3D(0.0f, 0.3f, 0.0f),
        QVector3D(0.3f, 0.0f, 0.0f),
        QVector3D(0.0f,0.3f,0.0f),
        QVector3D(0.0f,0.0f,0.3f)
    };

    for (int i = 0; i < limbNum; i++) {
        program->setUniformValue("m_color", color[i]);
        glDrawArrays(GL_TRIANGLES, i*6, 6);
    }

    animate();
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}


