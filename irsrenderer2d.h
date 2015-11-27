#ifndef IRSRENDERER2D_H
#define IRSRENDERER2D_H

#include <QWidget>

class IRSHandlerBase;

class IRSRenderer2D : public QWidget
{
    Q_OBJECT
public:
    explicit IRSRenderer2D(int id = 0, QWidget *parent = 0);
    void setHandler(IRSHandlerBase *handler);
    int getID();
    void setID(int id);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent *event);

private:
    int id;
    IRSHandlerBase *handler;

};

#endif // IRSRENDERER2D_H
