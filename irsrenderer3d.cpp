#include "irsrenderer3d.h"
#include "render_handler/irshandlerbase.h"

IRSRenderer3D::IRSRenderer3D(int id, QWidget *parent) :
    QOpenGLWidget(parent),
    id(id)
{

}

void IRSRenderer3D::setHandler(IRSHandlerBase *handler)
{
    this->handler = handler;
    handler->addView3D(id, this);
    this->update();
}

int IRSRenderer3D::getID()
{
    return this->id;
}

void IRSRenderer3D::setID(int id)
{
    this->id = id;
}

void IRSRenderer3D::initializeGL()
{
    initializeOpenGLFunctions();
    handler->init3D(id);
}

void IRSRenderer3D::paintGL()
{
    handler->paint3D(id);
}

void IRSRenderer3D::resizeGL(int w, int h)
{
    handler->viewResize3D(id, w, h);
}

void IRSRenderer3D::mousePressEvent(QMouseEvent *event)
{
    handler->mousePress3D(id, event);
}

void IRSRenderer3D::mouseMoveEvent(QMouseEvent *event)
{
    handler->mouseMove3D(id, event);
}

void IRSRenderer3D::mouseReleaseEvent(QMouseEvent *event)
{
    handler->mouseRelease3D(id, event);
}

void IRSRenderer3D::wheelEvent(QWheelEvent *event)
{
    handler->mouseWheel3D(id, event);
}

