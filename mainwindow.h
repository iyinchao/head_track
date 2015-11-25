#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

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
    IRSManager *manager;

private slots:
    void h_btIRSCtrl(bool checked);
    void h_mgrStream(bool isStart);
    void h_mgrError(IRSManager::IRSError error);
    void h_mgrSampleData(PXCCapture::Sample* sample);
};

#endif // MAINWINDOW_H
