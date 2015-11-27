#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

#include "irsrenderer2d.h"
#include "irsrenderer3d.h"
#include "render_handler/irshandlerhe.h"
#include "irsmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    IRSRenderer2D *colorView;
    IRSRenderer3D *depthView;
    IRSManager *manager;
    IRSHandlerBase *handler;

private slots:
    void h_btIRSCtrl(bool checked);
    void h_mgrStream(bool isStart);
    void h_mgrError(IRSManager::IRSError error);
//    void h_mgrSampleData(PXCCapture::Sample* sample);
};

#endif // MAINWINDOW_H
