#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    shapeNum = 0;

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
    static const float g_vertex_buffer_data[4][3] = {
        {-1.0f, -1.0f, 0.0f}, //bot left
        {-1.0f, 1.0f, 0.0f}, //top left
        {1.0f, -1.0f, 0.0f}, //bot right
        {1.0f,  1.0f, 0.0f} //top right
    };
    float coords[4][3];
    Limb* limb;
    moveLimb(g_vertex_buffer_data, coords, -1.0f, -1.0f, 0.0f, 0.10f);
    shapeNum++;
    limb = addLimb(coords);
    vbo.create();
    vbo.bind();
    vbo.allocate(coords, sizeof(coords));
}

Limb* GLWidget::addLimb(float coords[4][3]) {
    Limb* l = new Limb();
    l->setV1(coords[0][0], coords[0][1]);
    l->setV2(coords[1][0], coords[1][1]);
    l->setV3(coords[2][0], coords[2][1]);
    l->setV4(coords[3][0], coords[3][1]);
    body.addLimb(*l);
    return l;
}

Limb* GLWidget::addLimbAtJoint(Limb* parent, float coords[4][3], float jointX, float jointY) {
    Limb* l = new Limb();

    if (parent->jointSide(jointX, jointY) != Limb::Joint_Side::NON_JOINT) {


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
    program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));

    glDrawArrays(GL_TRIANGLE_FAN, 0, shapeNum * 4);

}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}


