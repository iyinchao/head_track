#ifndef IRSRENDERER2D_H
#define IRSRENDERER2D_H

#include <QWidget>

class IRSRenderer2D : public QWidget
{
    Q_OBJECT
public:
    explicit IRSRenderer2D(QWidget *parent = 0);
    void setRenderHandler();

signals:

public slots:

};

#endif // IRSRENDERER2D_H
