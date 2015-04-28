#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    limbNum = 0;

    setFocusPolicy(Qt::StrongFocus);
    body = Body();
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
            "out vec3 color;\n"
            "void main(void)\n"
            "{\n"
            "   color = vec3(1.0,0.0,0.0);\n"
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
    Limb* limb;
    moveLimb(g_vertex_buffer_data, coords, 0.0f, 0.0f, 0.0f, 0.10f);
    //limbNum++;
    limb = addLimb(coords);
    limb = addLimbAtJoint(limb, 0.0f, -0.1f, 0.1f, 0.5f);
    vbo.create();
    vbo.bind();
    vbo.allocate(body.getLimbVertices().constData(), body.getCount() * sizeof(float));
    //vbo.allocate(g_vertex_buffer_data, sizeof(g_vertex_buffer_data));
}

Limb* GLWidget::addLimb(float coords[4][3]) {
    Limb* l = new Limb();
    l->setV1(coords[0][0], coords[0][1]);
    l->setV2(coords[1][0], coords[1][1]);
    l->setV3(coords[2][0], coords[2][1]);
    l->setV4(coords[3][0], coords[3][1]);
    body.addLimb(*l);
    limbNum++;
    return l;
}

Limb* GLWidget::addLimbAtJoint(Limb* parent, float jointX, float jointY, float scaleX, float scaleY) {
    Limb* l = new Limb();
    Limb::Joint_Side js = parent->jointSide(jointX, jointY);
    if (js != Limb::Joint_Side::NON_JOINT) {
        l->setParent(parent);
        l->setJoint(jointX, jointY);
        if (js == Limb::Joint_Side::LEFT) {
            l->setV2(parent->getV1());
            l->setV3(parent->getV4());
            l->setV1((parent->getV1()) - QVector2D(scaleX,0));
            l->setV4((parent->getV4()) - QVector2D(scaleX,0));
        } else if (js ==  Limb::Joint_Side::RIGHT) {
            l->setV1(parent->getV2());
            l->setV4(parent->getV3());
            l->setV2((parent->getV2()) + QVector2D(scaleX,0));
            l->setV3((parent->getV3()) + QVector2D(scaleX,0));
        } else if (js == Limb::Joint_Side::BOTTOM) {
            l->setV4(parent->getV1());
            l->setV3(parent->getV2());
            l->setV1((parent->getV1()) - QVector2D(0,scaleY));
            l->setV2((parent->getV2()) - QVector2D(0,scaleY));
        } else {
            l->setV1(parent->getV4());
            l->setV2(parent->getV3());
            l->setV4((parent->getV4()) + QVector2D(0,scaleY));
            l->setV3((parent->getV3()) + QVector2D(0,scaleY));
        }
        body.addLimb(*l);
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

    glDrawArrays(GL_TRIANGLES, 0, limbNum * 6 * 3);

}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}


