#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    manager = &IRSManager::getInstance();

    connect(ui->bt_irsCtrl, SIGNAL(clicked(bool)), this, SLOT(h_btIRSCtrl(bool)));
    connect(manager, SIGNAL(_stream(bool)), this, SLOT(h_mgrStream(bool)));
    connect(manager, SIGNAL(_error(IRSManager::IRSError)), this, SLOT(h_mgrError(IRSManager::IRSError)));
    connect(manager, SIGNAL(_sampleData(PXCCapture::Sample*)), this, SLOT(h_mgrSampleData(PXCCapture::Sample*)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event){
    manager->stop();
    manager->wait();
    event->accept();
}

void MainWindow::h_btIRSCtrl(bool checked)
{
    ui->bt_irsCtrl->setText(tr("Please wait..."));
    ui->bt_irsCtrl->setEnabled(false);
    if(checked){
        manager->start();
    }else{
        manager->stop();
    }
}

void MainWindow::h_mgrStream(bool isStart)
{
    if(isStart){
        ui->bt_irsCtrl->setText(tr("Stop capture"));
        ui->bt_irsCtrl->setEnabled(true);
        ui->bt_irsCtrl->setChecked(true);
    }else{
        ui->bt_irsCtrl->setText(tr("Start capture"));
        ui->bt_irsCtrl->setEnabled(true);
        ui->bt_irsCtrl->setChecked(false);
    }
}

void MainWindow::h_mgrError(IRSManager::IRSError error)
{
    ui->bt_irsCtrl->setText(tr("Start capture"));
    ui->bt_irsCtrl->setEnabled(true);
    ui->bt_irsCtrl->setChecked(false);
}

void MainWindow::h_mgrSampleData(PXCCapture::Sample *sample)
{
    manager->lock.lockForRead();
    if(!manager->isRunning()){
        return;
    }



    manager->lock.unlock();
}
