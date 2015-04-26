#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QtGui>
#include <QMatrix4x4>
#include <QVector4D>
#include <QVector3D>

namespace Ui {
class GLWidget;
}

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

protected:
    virtual void paintGL() Q_DECL_OVERRIDE;
    virtual void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    virtual void initializeGL() Q_DECL_OVERRIDE;

    void initShaders();
    void initMat();

private:
    void loadLeg();

    QColor clearScreenColor;
    QOpenGLShaderProgram *program;
    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
    QMatrix4x4 mvpMat;
};

#endif // GLWIDGET_H
