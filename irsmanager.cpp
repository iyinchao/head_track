#include "irsmanager.h"

IRSManager::IRSManager()
    :isStop(true),
      session(NULL),
      senseManager(NULL),
      faceModule(NULL)
{
    _init_();
}

void IRSManager::_init_()
{
    if(session){
        session->Release();
    }
    session = PXCSession::CreateInstance();
    if(session == NULL){
        emit _error(IRSError::NO_SESSION);
    }
}

IRSManager &IRSManager::getInstance()
{
    static IRSManager instance;
    return instance;
}

void IRSManager::stop()
{
    isStop = true;
}

void IRSManager::start()
{
    //Renew senseManager
    senseManager = PXCSenseManager::CreateInstance();
    if(senseManager == NULL){
        emit _error(IRSError::NO_SENSE_MANAGER);
        return;
    }

    //Set senseManager mode
    senseManager->EnableFace();

    //config profiles
    PXCCaptureManager* captureManager = senseManager->QueryCaptureManager();
    PXCCapture::Device::StreamProfileSet set;
    memset(&set, 0, sizeof(set));
    set.color.imageInfo.height = 720;
    set.color.imageInfo.width = 1280;
    set.color.imageInfo.format =  PXCImage::PIXEL_FORMAT_RGB24;

    set.depth.imageInfo.height = 480;
    set.depth.imageInfo.width = 640;
    set.depth.imageInfo.format = PXCImage::PIXEL_FORMAT_DEPTH;
    captureManager->FilterByStreamProfiles(&set);


    //TODO: select device
//    PXCCapture *capture;
//    if (session->CreateImpl<PXCCapture>(&desc1, &capture) < PXC_STATUS_NO_ERROR)
    //See: E:\Documents\RSSDK\Samples\DF_FaceTracking\src\main.cpp: 113


    //Get faceModule
    faceModule = senseManager->QueryFace();
    if(faceModule == NULL){
        emit _error(IRSError::NO_FACE_MODULE);
        return;
    }

    //Config face detection
    config = faceModule->CreateActiveConfiguration();
    if(config == NULL){
        emit _error(IRSError::NO_FACE_CONFIG);
        return;
    }
    config->SetTrackingMode(PXCFaceConfiguration::TrackingModeType::FACE_MODE_COLOR_PLUS_DEPTH);
    config->landmarks.isEnabled = true;
    config->detection.isEnabled = true;
    config->pose.isEnabled = true;
    config->QueryPulse()->Disable();
    config->QueryExpressions()->DisableAllExpressions();
    config->QueryExpressions()->Disable();
    config->ApplyChanges();

    //Init capture
    if (senseManager->Init() < PXC_STATUS_NO_ERROR){
        emit _error(IRSError::INIT_FAILED);
        return;
    }
    faceData = faceModule->CreateOutput();

     if(!isRunning()){
         QThread::start();
     }

}

PXCSenseManager* IRSManager::getSenseManager()
{
    return senseManager;
}

void IRSManager::run()
{
    isStop = false;
    emit _stream(true);

    while(!isStop){

        if(senseManager->AcquireFrame(true) < PXC_STATUS_NO_ERROR){
            emit _error(ACQUIRE_FRAME);
            break;
        }

        faceData->Update();
        PXCCapture::Sample* sample = senseManager->QueryFaceSample();
        emit _sampleData(sample);

        lock.lockForWrite();
        senseManager->ReleaseFrame();
        lock.unlock();

    }

    config->Release();
    senseManager->Close();
    senseManager->Release();

    emit _stream(false);
}

