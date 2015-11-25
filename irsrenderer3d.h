#ifndef IRSRENDERER3D_H
#define IRSRENDERER3D_H

#include <QGLWidget>
#include <QPainter>
#include <GL/GLU.h>

class IRSRenderer3D : public QGLWidget
{
    Q_OBJECT
public:
    IRSRenderer3D();
    
protected:
    void initializeGL();
    void paintEvent(QPaintEvent* event);
    void resizeGL(int w, int h);
};

#endif // IRSRENDERER3D_H
