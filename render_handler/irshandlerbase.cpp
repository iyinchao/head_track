#include "irshandlerbase.h"

IRSHandlerBase::IRSHandlerBase(QWidget *parent) : QWidget(parent)
{
    manager = &IRSManager::getInstance();
    viewList2D = new QMap<int, IRSRenderer2D *>();
    viewList3D = new QMap<int, IRSRenderer3D *>();
}

void IRSHandlerBase::addView2D(int id, IRSRenderer2D *view)
{
    if(viewList2D->contains(id)){
        //TODO: handle modify value;
    }else{
        viewList2D->insert(id, view);
    }
}

void IRSHandlerBase::addView3D(int id, IRSRenderer3D *view)
{
    if(viewList3D->contains(id)){
        //TODO: handle modify value;
    }else{
        viewList3D->insert(id, view);
    }
}

PXCPoint3DF32 IRSHandlerBase::convertQtIRS(const QVector3D &qtVector)
{
    PXCPoint3DF32 irsVector;
    irsVector.x = qtVector.x();
    irsVector.y = qtVector.y();
    irsVector.z = qtVector.z();

    return irsVector;
}

QVector3D IRSHandlerBase::convertQtIRS(const PXCPoint3DF32 &irsVector)
{
    return QVector3D(irsVector.x, irsVector.y, irsVector.z);
}

void IRSHandlerBase::paint2D(int targetID, QPainter *painter, QPaintEvent *event)
{

}

void IRSHandlerBase::paint3D(int targetID)
{

}

void IRSHandlerBase::init3D(int targetID)
{

}

void IRSHandlerBase::viewResize2D(int targetID, QResizeEvent *event)
{

}

void IRSHandlerBase::viewResize3D(int targetID, int w, int h)
{

}

void IRSHandlerBase::mousePress3D(int targetID, QMouseEvent *event)
{

}

void IRSHandlerBase::mouseMove3D(int targetID, QMouseEvent *event)
{

}

void IRSHandlerBase::mouseRelease3D(int targetID, QMouseEvent *event)
{

}

void IRSHandlerBase::mouseWheel3D(int targetID, QWheelEvent *event)
{

}

void IRSHandlerBase::h_mgrSampleData(PXCCapture::Sample *sample, PXCFaceData *faceData)
{

}

