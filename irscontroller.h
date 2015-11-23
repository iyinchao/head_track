#ifndef IRSCONTROLLER_H
#define IRSCONTROLLER_H

#include <QThread>
#include <pxcsensemanager.h>

class IRSController : public QThread
{
    Q_OBJECT
public:
    IRSController();
    void stop();
    void start();

    void getSenseManager();

protected:
    void run();

private:
    PXCSenseManager *sm;


};

#endif // IRSCONTROLLER_H
