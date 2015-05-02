#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    limbNum = 0;

    setFocusPolicy(Qt::StrongFocus);
    body = Body();
    forward_leg = Leg_Forward::LEFT;
    ground_point = QVector2D(std::numeric_limits<float>::min(),
                             std::numeric_limits<float>::min());
    delta_radians = 0.0f;
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
    l = addLimb(coords);
    l->addNullParentJoint(); //add null parent joint since this limb has no parent
    addLimbAtJoint(*body.getLimbAt(0), 0.0f, -0.1f, 0.075f, 0.30f); //thigh 1
    addLimbAtJoint(*body.getLimbAt(0), 0.0f, -0.1f, 0.075f, 0.30f); //thigh 2
    l = addLimbAtJoint(*body.getLimbAt(1), 0.0f, -0.40f, 0.075f, 0.20f); //shin 1
    l->getJoint(0)->setMaxRot(0.0f);
    l->getJoint(0)->setMinRot(-M_PI/4.0f);
    l->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CW);
    l->rotateLimbAboutJoint(M_PI/4.0f, 0);
    l->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CCW);
    l = addLimbAtJoint(*body.getLimbAt(2), 0.0f, -0.40f, 0.075f, 0.20f); //shin 2
    l->getJoint(0)->setMaxRot(0.0f);
    l->getJoint(0)->setMinRot(-M_PI/4.0f);
    l->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CW);
    body.getLimbAt(2)->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CW);
    body.getLimbAt(0)->getJoint(1)->setRotationDirection(Joint::Rotation_Direction::CW);
    ground_point = QVector2D(0.0f,-0.6f);
    QVector<float> vertices = body.getLimbVertices();
    vbo.create();
    vbo.bind();
    vbo.allocate(vertices.constData(), body.getVertexCount() * sizeof(float));
    //vbo.allocate(g_vertex_buffer_data, sizeof(g_vertex_buffer_data));
}

void GLWidget::animate() {

    Limb* shin;
    if (forward_leg == Leg_Forward::LEFT &&
            body.getLimbAt(1)->getJoint(0)->getCurrRot() >= body.getLimbAt(1)->getJoint(0)->getMaxRot()) {
        body.getLimbAt(1)->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CW);
        body.getLimbAt(2)->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CCW);
        forward_leg = Leg_Forward::RIGHT;
    } else if (forward_leg == Leg_Forward::RIGHT &&
               body.getLimbAt(2)->getJoint(0)->getCurrRot() >= body.getLimbAt(2)->getJoint(0)->getMaxRot()) {
        body.getLimbAt(1)->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CCW);
        body.getLimbAt(2)->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CW);
        forward_leg = Leg_Forward::LEFT;
    }
    if (forward_leg == Leg_Forward::LEFT) {
        shin = body.getLimbAt(4);
        legForward(body.getLimbAt(1));
        legBackward(body.getLimbAt(2));

    } else {
        shin = body.getLimbAt(3);
        legForward(body.getLimbAt(2));
        legBackward(body.getLimbAt(1));
    }

    ground_point = QVector2D((shin->getV1().x() + shin->getV2().x())/2.0f,
                             (shin->getV1().y() + shin->getV2().y())/2.0f);
    moveBody(ground_point, shin->getParent()->getJoint(0)->getCurrRot());

    QVector<float> vertices = body.getLimbVertices();

    vbo.allocate(vertices.constData(), body.getVertexCount() * sizeof(float));
    update();
}

void GLWidget::legForward(Limb* leg) {

    leg->rotateLimbAboutJoint(M_PI/810.0f, 0);

    if (leg->getJoint(0)->getCurrRot() >= 0.0f) {
        leg->getChildren()[0]->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CCW);
        straightenKnee(leg->getChildren()[0]);
    }
    if (leg->getJoint(0)->getCurrRot() <= 0.0f) {
        leg->getChildren()[0]->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CW);
        bendKnee(leg->getChildren()[0]);
    }

}

void GLWidget::legBackward(Limb* leg) {
    leg->rotateLimbAboutJoint(M_PI/810.f, 0);
}

void GLWidget::bendKnee(Limb* shin) {
    shin->rotateLimbAboutJoint(M_PI/270.0f, 0);
}

void GLWidget::straightenKnee(Limb* shin) {
    shin->rotateLimbAboutJoint(M_PI/270.0f, 0);
}

void GLWidget::moveBody(QVector2D ground_point, float radians) {
    //delta_radians = radians - delta_radians;
    body.getLimbAt(0)->translateLimbY(ground_point, 0.6f, -0.1f, 0);
    body.getLimbAt(1)->translateLimbY(ground_point, 0.6f, -0.4f,0);
    body.getLimbAt(2)->translateLimbY(ground_point, 0.6f, -0.4f,0);
   // body.getLimbAt(0)->translateLimbX(M_PI/810.0f);
   // body.getLimbAt(1)->translateLimbX(M_PI/810.0f, 0);
   // body.getLimbAt(2)->translateLimbX(M_PI/810.0f, 0);
   // body.getLimbAt(3)->translateLimbX(M_PI/810.0f, 0);
    //body.getLimbAt(4)->translateLimbX(M_PI/810.0f, 0);
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
    Joint::Joint_Side js = parent.getJointSide(jointX, jointY);
    if (js != Joint::Joint_Side::NON_JOINT) {
        Limb* l = new Limb();
        parent.getChildren().append(l);
        parent.addChildJoint(jointX, jointY);
        parent.getJoint(parent.getJoints().count() - 1)->setJointSide(js);
        l->setParent(&parent);
        l->addParentJoint(jointX, jointY);
        if (js == Joint::Joint_Side::LEFT) {
            scaleDiff = (parent.getV4().y() - parent.getV1().y()) - scaleY;
            l->setV2(parent.getV1() + QVector2D(0.0f, scaleDiff/2.0f));
            l->setV3(parent.getV4() - QVector2D(0.0f, scaleDiff/2.0f));
            l->setV1((parent.getV1() + QVector2D(0.0f, scaleDiff/2.0f)) - QVector2D(scaleX,0));
            l->setV4((parent.getV4() - QVector2D(0.0f, scaleDiff/2.0f)) - QVector2D(scaleX,0));
            l->getJoint(l->getJoints().count() - 1)->setJointSide(Joint::Joint_Side::RIGHT);
        } else if (js ==  Joint::Joint_Side::RIGHT) {
            scaleDiff = (parent.getV3().y() - parent.getV2().y()) - scaleY;
            l->setV1(parent.getV2() + QVector2D(0.0f, scaleDiff/2.0f));
            l->setV4(parent.getV3() - QVector2D(0.0f, scaleDiff/2.0f));
            l->setV2((parent.getV2() + QVector2D(0.0f, scaleDiff/2.0f)) + QVector2D(scaleX,0));
            l->setV3((parent.getV3() - QVector2D(0.0f, scaleDiff/2.0f)) + QVector2D(scaleX,0));
            l->getJoint(l->getJoints().count() - 1)->setJointSide(Joint::Joint_Side::LEFT);
        } else if (js == Joint::Joint_Side::BOTTOM) {
            scaleDiff = (parent.getV2().x() - parent.getV1().x()) - scaleX;
            l->setV4(parent.getV1() + QVector2D(scaleDiff/2.0f, 0.0f));
            l->setV3(parent.getV2() - QVector2D(scaleDiff/2.0f, 0.0f));
            l->setV1((parent.getV1() + QVector2D(scaleDiff/2.0f, 0.0f)) - QVector2D(0,scaleY));
            l->setV2((parent.getV2() - QVector2D(scaleDiff/2.0f, 0.0f)) - QVector2D(0,scaleY));
            l->getJoint(l->getJoints().count() - 1)->setJointSide(Joint::Joint_Side::TOP);
        } else {
            scaleDiff = (parent.getV3().x() - parent.getV4().x()) - scaleX;
            l->setV1(parent.getV4() + QVector2D(scaleDiff/2.0f, 0.0f));
            l->setV2(parent.getV3() - QVector2D(scaleDiff/2.0f, 0.0f));
            l->setV4((parent.getV4() + QVector2D(scaleDiff/2.0f, 0.0f)) + QVector2D(0,scaleY));
            l->setV3((parent.getV3() - QVector2D(scaleDiff/2.0f, 0.0f)) + QVector2D(0,scaleY));
            l->getJoint(l->getJoints().count() - 1)->setJointSide(Joint::Joint_Side::BOTTOM);
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


