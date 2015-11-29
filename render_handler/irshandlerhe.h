#ifndef IRSHANDLERHE_H
#define IRSHANDLERHE_H

#include <QMouseEvent>
#include <QWheelEvent>
#include <QPushButton>
#include "irshandlerbase.h"
#include "../cglutil.h"

class IRSHandlerHE : public IRSHandlerBase
{
    Q_OBJECT
public:
    IRSHandlerHE(QWidget *parent = 0);

public slots:
    void paint2D(int targetID, QPainter *painter, QPaintEvent *event);
    void init3D(int targetId);
    void paint3D(int targetID);
    void viewResize3D(int targetID, int w, int h);
    void mousePress3D(int targetID, QMouseEvent *event);
    void mouseMove3D(int targetID, QMouseEvent *event);
    void mouseRelease3D(int targetID, QMouseEvent *event);
    void mouseWheel3D(int targetID, QWheelEvent *event);
    void h_mgrSampleData(PXCCapture::Sample* sample, PXCFaceData *faceData = 0);
    void h_btFPV_(bool checked);
    void h_btSideView_();
    void h_btTopView_();

private:
    PXCCapture::Sample* sample;
    PXCImage::ImageInfo colorInfo;
    PXCImage::ImageData colorData;
    PXCImage *colorImage;
    PXCFaceData *faceData;
    PXCSmoother::Smoother3D *smootherHPos;
    PXCSmoother::Smoother3D *smootherHRot;

    QPoint v_lastPos;
    bool isMouseDown;
    double v_theta;
    double v_phi;
    double v_cx;
    double v_cy;
    double v_cz;
    double v_r;
    double v_r_max;
    double v_r_min;
    bool isFPV;
    bool isNoValidBefore;
    QVector3D irsPos;
    QVector3D targetPos;
    QVector3D v_t_up;
    QVector3D v_t_center;
    QVector3D v_t_eye;

    QPushButton *btFPV;
    QPushButton *btTopView;
    QPushButton *btSideView;
};

#endif // IRSHANDLERHE_H
