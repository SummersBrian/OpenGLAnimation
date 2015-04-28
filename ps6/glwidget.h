#ifndef GLWidget_H
#define GLWidget_H

#include "body.h"
#include "limb.h"

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
    void loadBody();
    void moveLimb(const float coords[4][3], float (&returnCoords)[4][3], float x, float y, float z, float scale);
    Limb* addLimb(float coords[4][3]);
    Limb* addLimbAtJoint(Limb* parent, float jointX, float jointY, float scaleX, float scaleY);

    QColor clearColor;
    QOpenGLShaderProgram *program;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QMatrix4x4 proj;
    QMatrix4x4 view;
    QMatrix4x4 model;
    int limbNum;
    Body body;

#define CUBE_NUM 1
};

#endif  //GLWidget_h
