#ifndef IRSRENDERER3D_H
#define IRSRENDERER3D_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QPainter>
#include <GL/GLU.h>

class IRSHandlerBase;

class IRSRenderer3D : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    IRSRenderer3D(int id = 0, QWidget *parent = 0);
    void setHandler(IRSHandlerBase *handler);
    int getID();
    void setID(int id);

signals:

public slots:
    
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    int id;
    IRSHandlerBase *handler;
};

#endif // IRSRENDERER3D_H
