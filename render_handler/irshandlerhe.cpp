#include "irshandlerhe.h"

IRSHandlerHE::IRSHandlerHE(QWidget *parent):
    IRSHandlerBase(parent),
    sample(NULL),
    faceData(NULL),
    isFPV(false),
    isNoValidBefore(true)
{
    v_r = 0.5;
    v_r_max = 10;
    v_r_min = 0.1;
    v_theta = 80;
    v_phi = 0;
    v_cx = 0;
    v_cy = 0;
    v_cz = 0.4;
    v_lastPos = QPoint(0, 0);
    v_t_center = QVector3D(0,0,0);
    v_t_eye = QVector3D(0,0,0.5);
    v_t_up = QVector3D(0,1,0);
    irsPos = QVector3D(0, 1.2, -2); //realsense position relative to car corrd
    targetPos = QVector3D(0, 0, -6); //target position relative to car corrd
    isMouseDown = false;

    QUiLoader loader;
    QFile file(":/forms/handlerHE");
    file.open(QFile::ReadOnly);
    QWidget *w = loader.load(&file, this);
    file.close();
    qobject_cast<QGridLayout *>(this->parentWidget()->layout())->addWidget(w, 0,0,1,1);

    btFPV = w->findChild<QPushButton *>("bt_he_FPV");
    btTopView = w->findChild<QPushButton *>("bt_he_topView");
    btSideView = w->findChild<QPushButton *>("bt_he_sideView");
    connect(btFPV, SIGNAL(clicked(bool)), this, SLOT(h_btFPV_(bool)));
    connect(btTopView, SIGNAL(clicked()), this, SLOT(h_btTopView_()));
    connect(btSideView, SIGNAL(clicked()), this, SLOT(h_btSideView_()));

    PXCSmoother* smoother = NULL;
    manager->getSession()->CreateImpl<PXCSmoother>(&smoother);
    smootherHPos = smoother->Create3DQuadratic(0.5f);
    smootherHRot = smoother->Create3DQuadratic(0.5f);
    smootherHPos->Reset();
    smootherHRot->Reset();
}

void IRSHandlerHE::paint2D(int targetID, QPainter *painter, QPaintEvent *event)
{
    manager->lock.lockForRead();

    if(!viewList2D->contains(targetID)){
        return;
    }
    IRSRenderer2D *view = viewList2D->value(targetID);

    switch(targetID){
    case 0:
        if(sample){
            QImage img;
            img = QImage(colorData.planes[0], 1280, 720, QImage::Format_RGB32);
            painter->drawPixmap(0,0, QPixmap::fromImage(img.scaledToHeight(view->height())));
            colorImage->ReleaseAccess(&colorData);

            double ratio = view->height() / 720.0f;
            if(this->faceData && this->faceData->QueryNumberOfDetectedFaces() > 0){
                PXCRectI32 rect;
                if(faceData->QueryFaceByIndex(0)->QueryDetection()->QueryBoundingRect(&rect)){
                    pen.setColor(Qt::yellow);
                    pen.setWidth(2);
                    painter->setPen(pen);
                    painter->drawRect(rect.x * ratio, rect.y * ratio, rect.w * ratio, rect.h * ratio);
                }
            }
            sample = NULL;
            faceData = NULL;
        }else{
            //resize
        }
        break;
    }

    manager->lock.unlock();

}

void IRSHandlerHE::init3D(int targetId)
{
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_MULTISAMPLE);
    glClearColor(0.0f, 0.282f, 0.4f, 0.5f);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glLoadIdentity();
    static GLfloat lightPosition[4] = { 0, 0, -20.0, 1.0 };
    GLfloat lightDiffuse[]   = {1.0, 1.0, 1.0, 1.0};
    GLfloat lightSpecular[] = {0.5, 0.5, 0.5, 1.0};
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);
}

void IRSHandlerHE::paint3D(int targetID)
{
    manager->lock.lockForRead();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    if(isFPV){
        gluLookAt(-v_t_eye.x() + irsPos.x(), v_t_eye.y() + irsPos.y(), v_t_eye.z() + irsPos.z(),
                  -v_t_center.x() + irsPos.x(), v_t_center.y() + irsPos.y(), v_t_center.z() + irsPos.z(),
                  0, 1, 0);
                  //-v_t_up.x(), v_t_up.y(), v_t_up.z());
    }else{
        gluLookAt(v_r*qSin(v_theta*M_PI/180)*qSin(v_phi*M_PI/180) + -v_t_center.x() + irsPos.x(),
                      v_r*qCos(v_theta*M_PI/180)  + v_t_center.y() + irsPos.y(),
                      v_r*qSin(v_theta*M_PI/180)*qCos(v_phi*M_PI/180)  + v_t_center.z() + irsPos.z(),
                      -v_t_center.x() + irsPos.x(), v_t_center.y() + irsPos.y(), v_t_center.z() + irsPos.z(),
                      0, 1, 0);
    }


    //draw car axis
    CGLUtil::drawAxis(0.15, 1.5);
    glColor4f(0.0f, 0.6f, 0.8f, 0.2f);

    //darw ground
    glPushMatrix();
    glTranslatef(0, 0, -5);
    CGLUtil::drawWiredPlane(10, 10, 20, 20);
    glPopMatrix();

    //draw target
    glPushMatrix();
    glTranslatef(targetPos.x(), targetPos.y(), targetPos.z());
    CGLUtil::drawAxis(0.5, 0.3, QColor(255,0,0),QColor(255,0,0),QColor(255,0,0),true,true,false,false);
    glScalef(-1, -1, 1);
    CGLUtil::drawAxis(0.5, 0.3, QColor(255,0,0),QColor(255,0,0),QColor(255,0,0),true,true,false,false);
    glPopMatrix();

    glPushMatrix(); //big one

    glTranslatef(irsPos.x(), irsPos.y(), irsPos.z()); //below are all realtive to realsense

    //draw realsense plane
    glPushMatrix();
    glTranslatef(0, 0, 1);
    glColor3f(0.0f, 0.45f, 0.6f);
    CGLUtil::drawWiredPlane(2, 2, 20, 20);
    glPopMatrix();

    //draw realsense
    CGLUtil::drawAxis(0.1);
    glColor4f(1, 1, 1, 0.4);
    CGLUtil::drawBox(0.15, 0.02, 0.025);

    if(this->faceData && this->faceData->QueryNumberOfDetectedFaces() > 0 && faceData->QueryFaceByIndex(0)->QueryLandmarks()){
        int lmNum = faceData->QueryFaceByIndex(0)->QueryLandmarks()->QueryNumPoints();
        PXCFaceData::LandmarkPoint* lmp = new PXCFaceData::LandmarkPoint[lmNum];
        PXCFaceData::PoseQuaternion pea;
        if(faceData->QueryFaceByIndex(0)->QueryLandmarks()->QueryPoints(lmp) &&
                faceData->QueryFaceByIndex(0)->QueryPose()->QueryPoseQuaternion(&pea)){

            int leftEyeIndex = faceData->QueryFaceByIndex(0)->QueryLandmarks()->QueryPointIndex(PXCFaceData::LandmarkType::LANDMARK_EYE_LEFT_CENTER);
            int rightEyeIndex = faceData->QueryFaceByIndex(0)->QueryLandmarks()->QueryPointIndex(PXCFaceData::LandmarkType::LANDMARK_EYE_RIGHT_CENTER);

            QVector3D eyeCenter((lmp[leftEyeIndex].world.x + lmp[rightEyeIndex].world.x) / 2,
                                (lmp[leftEyeIndex].world.y + lmp[rightEyeIndex].world.y) / 2,
                                (lmp[leftEyeIndex].world.z + lmp[rightEyeIndex].world.z) / 2);


            QQuaternion quat(QVector4D(pea.z, pea.y, pea.x, pea.w));
            float x,y,z,angle;
            quat.getAxisAndAngle(&x, &y,&z, &angle);
            if(lmp[leftEyeIndex].confidenceWorld > 0){
                QVector3D rotRay = quat.rotatedVector(QVector3D(0,0,-0.1));
                QVector3D rotUp = quat.rotatedVector(QVector3D(0,0.1,0));
                if(isNoValidBefore){
                    smootherHRot->Reset();
                    smootherHPos->Reset();
                    isNoValidBefore = false;
                }
                PXCPoint3DF32 smoothedPoint = smootherHRot->SmoothValue(convertQtIRS(rotRay));
                rotRay = convertQtIRS(smoothedPoint);
                smoothedPoint = smootherHPos->SmoothValue(convertQtIRS(eyeCenter));
                eyeCenter = convertQtIRS(smoothedPoint);
                v_t_center = eyeCenter + rotRay;
                v_t_eye = eyeCenter;
                v_t_up = eyeCenter + rotUp;
            }else{
                isNoValidBefore = true;
            }
            glPushMatrix();
            if(!isFPV){
                glScalef(-1, 1, 1);// change coord
                //landmarks
                for(int i = 0; i < lmNum; i++){
                    glPushMatrix();
                    glTranslated(lmp[i].world.x, lmp[i].world.y, lmp[i].world.z);
                    if(i == leftEyeIndex ||
                       i == rightEyeIndex){
                        glColor4f(1, 0, 0, 1);
                        CGLUtil::drawSphere(0.004, 18, 9);
                    }else{
                        glColor4f(1, 1, 1, 1);
                        CGLUtil::drawSphere(0.0015, 18, 9);
                    }
                    glPopMatrix();
                }
                //eye connect line
                glPushMatrix();
                glBegin(GL_LINES);
                glVertex3f(lmp[leftEyeIndex].world.x, lmp[leftEyeIndex].world.y, lmp[leftEyeIndex].world.z);
                glVertex3f(lmp[rightEyeIndex].world.x, lmp[rightEyeIndex].world.y, lmp[rightEyeIndex].world.z);
                glEnd();
                glPopMatrix();
                //
//                glBegin(GL_LINES);
//                glVertex3f(eyeRayPoint.x(), eyeRayPoint.y(), eyeRayPoint.z());
//                glVertex3f(eyeCenter.x(), eyeCenter.y(), eyeCenter.z());
//                glEnd();
                //eye direction
                glPushMatrix();
                glTranslatef(eyeCenter.x(), eyeCenter.y(), eyeCenter.z());
                glRotatef(angle, x, y, z);
                glScalef(1,1,-1);
                CGLUtil::drawAxis(0.1, 1.5, QColor(100, 100, 100), QColor(100, 100, 100), QColor(255, 255, 100), false, false);
                glPopMatrix();
            }
            glPopMatrix();
            //draw HUD
            QVector3D p1 = eyeCenter;
            QVector3D p2 = targetPos - irsPos;
            y = ((p1.y() * (p2.z() - p1.z()) - p1.z() * (p2.y() - p1.y()))) / ((p2.z() - p1.z()) - (p2.y() - p1.y()));
            z = y;
            x = (y - p1.y()) * (p2.x() - p1.x()) / (p2.y() - p1.y()) + p1.x();
            glPushMatrix();
            glTranslatef(-x, y, z);
            glPushMatrix();
            CGLUtil::drawAxis(0.2, 0.2, QColor(0,255,0,100),QColor(0,255,0,100),QColor(0,255,0,100),true,true,false,false);
            glScalef(-1, -1, 1);
            CGLUtil::drawAxis(0.2, 0.2, QColor(0,255,0,100),QColor(0,255,0,100),QColor(0,255,0,100),true,true,false,false);
            glPopMatrix();
            glPopMatrix();
        }

        delete lmp;
        lmp = NULL;
    }
    //draw glass screen
    glColor4f(1,1,1,0.05f);
    glBegin(GL_POLYGON);
    glVertex3f(-0.5, 0, 0);
    glVertex3f(-0.5, -0.6, -0.6);
    glVertex3f(1, -0.6, -0.6);
    glVertex3f(1, 0, 0);
    glVertex3f(-0.5, 0, 0);
    glEnd();

    glPopMatrix();

    manager->lock.unlock();
}

void IRSHandlerHE::viewResize3D(int targetID, int w, int h)
{
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, w / double(h), 0.001, 15000);
    glMatrixMode(GL_MODELVIEW);
}

void IRSHandlerHE::mousePress3D(int targetID, QMouseEvent *event)
{
    isMouseDown = true;
    v_lastPos = event->pos();
}

void IRSHandlerHE::mouseMove3D(int targetID, QMouseEvent *event)
{

    if(!isMouseDown) return;
    if(!isFPV){
        int dx = event->x() - v_lastPos.x();
        int dy = event->y() - v_lastPos.y();
        if (event->buttons() & Qt::LeftButton) {
            v_phi -= dx / 2;
            if(v_phi > 360) v_phi -= 360;
            if(v_phi < 0) v_phi += 360;
            v_theta -= dy;
            if(v_theta > 180) v_theta = 180;
            if(v_theta < 1) v_theta = 1;
        }
        viewList3D->value(targetID)->update();
        v_lastPos = event->pos();
    }
}

void IRSHandlerHE::mouseRelease3D(int targetID, QMouseEvent *event)
{
    isMouseDown = false;
}

void IRSHandlerHE::mouseWheel3D(int targetID, QWheelEvent *event)
{
    if(!isFPV){
        v_r += event->delta() /double(480);
        if(v_r > v_r_max)
            v_r = v_r_max;
        if(v_r < v_r_min)
            v_r = v_r_min;
        viewList3D->value(targetID)->update();
    }
}

void IRSHandlerHE::h_mgrSampleData(PXCCapture::Sample *sample, PXCFaceData *faceData)
{

    manager->lock.lockForRead();
    if(!manager->isRunning()){
        return;
    }

    this->sample = sample;
    this->faceData = faceData;

    QMap<int, IRSRenderer2D *>::const_iterator i = viewList2D->constBegin();
    while (i != viewList2D->constEnd()) {
        if(i.value()->isVisible()){
            colorImage = sample->color;
            pxcStatus status = colorImage->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_RGB32, &colorData);
            if (status >= PXC_STATUS_NO_ERROR){
                i.value()->update();
            }else{
                qDebug()<<status;
            }

        }
        ++i;
    }

    IRSRenderer3D *view3D =  viewList3D->value(0);
    if(view3D->isVisible()){
        view3D->update();
    }

    manager->lock.unlock();
}

void IRSHandlerHE::h_btFPV_(bool checked)
{
    isFPV = checked;
    viewList3D->value(0)->update();
    if(checked){

    }else{

    }
}

void IRSHandlerHE::h_btSideView_()
{
    btFPV->setChecked(false);
    isFPV = false;
    v_theta = 90;
    v_phi = 270;
    viewList3D->value(0)->update();
}

void IRSHandlerHE::h_btTopView_()
{
    btFPV->setChecked(false);
    isFPV = false;
    v_theta = 0.0001;
    v_phi = 0;
    viewList3D->value(0)->update();
}

