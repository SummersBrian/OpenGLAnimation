/****************************************************************************
**
CMSC427 Spring 2015 Problem Set 2
Brian Summers - 110656609
summers.brian.cs@gmail.com
3/5/2015


Uses Start code by:
Window for OpenGL in QT.
Start code for CMSC 427, Spring 2015
Reference: cube & texture example in Qt Creator
author: Zheng Xu, xuzhustc@gmail.com
**
****************************************************************************/

#include "GLWidget.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    cubNum = 0;

    setFocusPolicy(Qt::StrongFocus);

    //transformation variables
    rot_radians = 3.14f/36.0f;
    automove = false;
    move_speed = 0.03f;
    rot_count = 0;
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
    loadShapes();
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

void GLWidget::loadShapes()
{
    static const GLfloat g_vertex_buffer_data[3][3] = {
        {-1.0f, -1.0f, 0.0f},
        {1.0f, -1.0f, 0.0f},
        {0.0f,  1.0f, 0.0f}
    };
    GLfloat coords[3][3];
    scaleShapes(g_vertex_buffer_data, coords,0.25f);

    vbo.create();
    vbo.bind();
    vbo.allocate(coords, sizeof(coords));
}

void GLWidget::scaleShapes(const GLfloat coords[3][3], GLfloat (&returnCoords)[3][3], float scale){
    for (int i = 0; i < 3; i++) {
            returnCoords[i][0] = coords[i][0] * scale;
            returnCoords[i][1] = coords[i][1] * scale;
            returnCoords[i][2] = coords[i][2] * scale;
    }
}

void GLWidget::paintGL()
{
    //background
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));

    glDrawArrays(GL_TRIANGLES, 0, 3);

}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}


