#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{

}

GLWidget::~GLWidget()
{
    makeCurrent();
    vbo.destroy();
    delete program;
    doneCurrent();

}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    clearScreenColor.setRgbF(0.1,0.1,0.1,0.0);
    vao.create();
    vao.bind();
    loadLeg();
    initMat();
    initShaders();
}

void GLWidget::initMat() {
    QMatrix4x4 proj;
    QMatrix4x4 view;
    proj.ortho(0.0f,50.0f,0.0f,50.0f,0.0f,2.0f);
    view.lookAt(QVector3D(0.0f, 0.0, 0.0f), QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f));
    mvpMat = proj * view;
}

void GLWidget::initShaders()
{
#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    const char *vsrc =
            "#version 330\n"
            "layout (location = 0) in vec4 vertex;\n"
            "layout (location = 1) in vec4 texCoord;\n"
            "uniform mat4 matrix;\n"
            "out vec4 tCd;\n"
            "void main(void)\n"
            "{\n"
            "   gl_Position = matrix * vertex;\n"
            "   tCd = texCoord;\n"
            "}\n";
    vshader->compileSourceCode(vsrc);

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    const char *fsrc =
            "#version 330\n"
            "uniform sampler2D fTexture;\n"
            "in vec4 tCd;\n"
            "out vec4 ffColor;\n"
            "void main(void)\n"
            "{\n"
            "   ffColor = texture(fTexture, tCd.st);\n"
            "}\n";
    fshader->compileSourceCode(fsrc);

    program = new QOpenGLShaderProgram;
    program->addShader(vshader);
    program->addShader(fshader);
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->link();

    program->bind();
    program->setUniformValue("fTexture", 0);
}



void GLWidget::resizeGL(int w, int h)
{
    int side = qMin(w, h);
    glViewport((w - side) / 2, (h - side) / 2, side, side);
}

void GLWidget::paintGL()
{
    //background
    glClearColor(clearScreenColor.redF(), clearScreenColor.greenF(), clearScreenColor.blueF(), clearScreenColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program->setUniformValue("matrix", mvpMat);
    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

    for (int i = 0; i < 1; ++i) {
        //textures[i]->bind();
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }

}

void loadLeg() {

}
