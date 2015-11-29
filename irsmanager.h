#ifndef IRSCONTROLLER_H
#define IRSCONTROLLER_H

#include <QThread>
#include <pxcsensemanager.h>
#include <utilities/pxcsmoother.h>
#include <QReadWriteLock>

#include <QDebug>

class IRSManager : public QThread
{
    Q_OBJECT
public:
    //
    enum IRSError{
        NO_SESSION = 0x0,
        NO_SENSE_MANAGER = 0x01,
        NO_FACE_MODULE = 0x02,
        INIT_FAILED = 0x04,
        NO_FACE_CONFIG = 0x08,
        ACQUIRE_FRAME = 0x10
    };
    enum IRSResType{

    };

    static IRSManager& getInstance();
    PXCSenseManager *getSenseManager();
    PXCSession *getSession();
    QReadWriteLock lock;

signals:
    void _error(IRSManager::IRSError errorCode);
    void _stream(bool isStart);
    void _sampleData(PXCCapture::Sample* sample, PXCFaceData *faceData = 0);

public slots:
    void stop();
    void start();

protected:
    void run();

private:
    IRSManager();
    IRSManager(IRSManager const&);
    IRSManager& operator=(IRSManager const&);

    void _init_();

    bool isStop;
    PXCSession *session;
    PXCSenseManager *senseManager;
    PXCFaceModule *faceModule;
    PXCFaceData *faceData;
    PXCFaceConfiguration *config;

};

#endif // IRSCONTROLLER_H
