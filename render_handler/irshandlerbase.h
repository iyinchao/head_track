#ifndef IRSHANDLERBASE_H
#define IRSHANDLERBASE_H

#include <QWidget>
#include <QOpenGLWidget>
#include "../irsmanager.h"
#include "../irsrenderer2d.h"
#include "../irsrenderer3d.h"

class IRSHandlerBase : public QWidget
{
    Q_OBJECT
public:
    explicit IRSHandlerBase(QWidget *parent = 0);
    void addView2D(int id, IRSRenderer2D *view);
    void addView3D(int id, IRSRenderer3D *view);

public slots:
    virtual void paint2D(int targetID, QPainter *painter, QPaintEvent *event);
    virtual void paint3D(int targetID);
    virtual void init3D(int targetID);
    virtual void viewResize2D(int targetID, QResizeEvent* event);
    virtual void viewResize3D(int targetID, int w, int h);
    virtual void mousePress3D(int targetID, QMouseEvent *event);
    virtual void mouseMove3D(int targetID, QMouseEvent *event);
    virtual void mouseRelease3D(int targetID, QMouseEvent *event);
    virtual void mouseWheel3D(int targetID, QWheelEvent *event);
    virtual void h_mgrSampleData(PXCCapture::Sample* sample, PXCFaceData *faceData = 0);

signals:


protected:
    IRSManager *manager;
    QMap<int, IRSRenderer2D *> *viewList2D;
    QMap<int, IRSRenderer3D *> *viewList3D;
    QPen pen;
};

#endif // IRSHANDLERBASE_H
