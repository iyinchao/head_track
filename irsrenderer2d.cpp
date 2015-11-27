#include "irsrenderer2d.h"
#include "render_handler/irshandlerbase.h"

IRSRenderer2D::IRSRenderer2D(int id, QWidget *parent) :
    QWidget(parent),
    handler(NULL),
    id(id)
{
    
}

void IRSRenderer2D::setHandler(IRSHandlerBase *handler)
{
    this->handler = handler;
    handler->addView2D(id, this);
    this->repaint();
}

int IRSRenderer2D::getID()
{
    return this->id;
}

void IRSRenderer2D::setID(int id)
{
    this->id = id;
}

void IRSRenderer2D::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);

    if(handler){
        handler->paint2D(id, &painter, event);
    }

    painter.end();
}

void IRSRenderer2D::resizeEvent(QResizeEvent *event)
{
    if(handler){
        handler->viewResize2D(id, event);
    }
}

