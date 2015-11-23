#include "mainwindow.h"
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    PXCSenseManager *sm = PXCSenseManager::CreateInstance();
    PXCSession *session = sm->QuerySession();
    //session->SetCoordinateSystem(PXCSession::COORDINATE_SYSTEM_REAR_OPENCV);
    sm->EnableStream(PXCCapture::STREAM_TYPE_DEPTH,0,0);
    sm->Init();

//    qDebug()<<sm->QueryCaptureManager()->QueryDevice()->QueryDepthFieldOfView().x;

    for (;;) {
        pxcStatus sts=sm->AcquireFrame(true);
        if (sts<PXC_STATUS_NO_ERROR)
            break;
        PXCCapture::Sample *sample;
        sample=sm->QuerySample();
        sm->ReleaseFrame();
        if(session->QueryCoordinateSystem() & PXCSession::COORDINATE_SYSTEM_FRONT_DEFAULT){
            qDebug()<<"1";
        }
        if(session->QueryCoordinateSystem() & PXCSession::COORDINATE_SYSTEM_REAR_OPENCV){
            qDebug()<<"2";
        }
    }

    return a.exec();
}
