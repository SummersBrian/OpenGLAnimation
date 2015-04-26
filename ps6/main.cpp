#ifndef QT_NO_OPENGL
#include "glwidget.h"
#endif

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setVersion(3,3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    a.setApplicationName("PS6");

#ifndef QT_NO_OPENGL
    GLWidget w;
    w.show();
#else
    QLabel note("OpenGL Support required");
    note.show;
#endif
    return a.exec();
}
