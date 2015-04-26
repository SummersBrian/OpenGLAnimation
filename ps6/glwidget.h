/****************************************************************************
**
CMSC427 Spring 2015 Problem Set 2
Brian Summers - 110656609
summers.brian.cs@gmail.com
3/5/2015

Uses start code by:
Window for OpenGL in QT.
Start code for CMSC 427, Spring 2015
Reference: cube & texture example in Qt Creator
author: Zheng Xu, xuzhustc@gmail.com
**
****************************************************************************/

#ifndef GLWidget_H
#define GLWidget_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QKeyEvent>
#include <QtGui>
#include <QtCore/qmath.h>

#include <QMatrix4x4>
#include <QVector4D>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

    void initShaders();

private:
    void loadShapes();
    void scaleShapes(const GLfloat coords[3][3], GLfloat (&returnCoords)[3][3], float scale);

    QColor clearColor;
    QOpenGLShaderProgram *program;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QMatrix4x4 proj;
    QMatrix4x4 view;
    QMatrix4x4 model;
    QTimer *timer, *timer2;

    float move_speed;
    float rot_radians;
    int rot_count;
    int cubNum;
    bool automove;
    QString txtPath;

#define RESOURCE_FLAG true
#define TXT_IMG_PATH "C:/Users/Zheng/Documents/openglTest/images"
#define CUBE_NUM 1
};

#endif  //GLWidget_h
