#include "dialog.h"
#include <QPainter>
#include <QTimer>
#include <QThread>
#include<QDebug>
#include "cnet.h"
//#include "qwt_picker.h"
//#include "work.h"


#include <QMouseEvent>
//#include "vars.h"
int mouse_ind;
bool lets_drop;
float my_scale=1.5;
int rad=4;
float f;
QTimer *timer;
CNet net(50,RS);
//work* WK;

void getArrows()
{
    for(int i=0;i<net.size;i++)
        for(int j=0;j<net.size;j++)
        {
            if(net.neuron[i].weight[j]>0)
            {

            }
        }


}

Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{
    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawing()));
    timer->start(40);
    this->update();
    //    QThread* thread = new QThread( );
    //    WK=new work();
    //    WK->moveToThread(thread);
    //    connect(thread,SIGNAL(started()),WK,SLOT(doWork()));
    //    thread->start();

}

void Dialog::drawing()
{
    this->update();
}

void Dialog::mouseReleaseEvent(QMouseEvent *e)
{
    lets_drop=0;
}

void Dialog::mouseMoveEvent(QMouseEvent *e)
{
    if(lets_drop)
    {
        net.neuron[mouse_ind].x=(e->x()/my_scale);
        net.neuron[mouse_ind].y=(e->y()/my_scale);
    }
}

void Dialog::mousePressEvent(QMouseEvent *e)
{
    QPointF MouseP=(e->pos()/my_scale);//works in origin space
    QPointF V;
    qDebug()<<MouseP.x();
    for(int i=0;i<net.size;i++)
    {
        V=MouseP-QPointF(net.neuron[i].x,net.neuron[i].y);
        if(QPointF::dotProduct(V,V)<20)
        {
            mouse_ind=i;
            lets_drop=1;
            net.neuron[i].vis=200;
        }
    }

}

void Dialog::mainCircle()
{

    net.test();


}

void Dialog::paintEvent(QPaintEvent* e)
{
    static float t=1;
    t+=.06;
    if(t>50)t=10;
    for (int i=0;i<50;i++)
        mainCircle();

    QPainter* painter=new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing, 1);
    //    QPen pen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen pen(Qt::black);
    painter->setPen(pen);
    painter->scale(my_scale,my_scale);




    for(int i=0;i<net.size;i++)
        for(int j=0;j<net.size;j++)
        {
            float h=net.neuron[i].weight[j]*250;

            if(h>0)
            {
                pen.setColor(QColor(h,h,h));
                painter->setPen(pen);
                painter->drawLine(net.neuron[i].x,net.neuron[i].y,net.neuron[j].x,net.neuron[j].y);
            }
        }


    //    pen.setWidth(4);
    //    pen.setColor(QColor(0,0,0));
    //    painter->setPen(pen);


    for(int i=0;i<net.size;i++)
    {
        QPainterPath path;
        path.addEllipse ( QPointF(net.neuron[i].x,net.neuron[i].y),  rad,  rad );
        QRadialGradient gradient=QRadialGradient(QPointF(net.neuron[i].x,net.neuron[i].y),rad,
                                                 QPointF(net.neuron[i].x,net.neuron[i].y)+QPointF(rad,rad));
        //     gradient(0, 0, 0, 100);
        gradient.setColorAt(1.0, Qt::black);
        gradient.setColorAt(0.0, QColor(0,net.neuron[i].vis,0));
        //    painter->fillPath(path,QBrush(QColor(0,0,0)));
        painter->fillPath(path,gradient);
    }


    delete painter;
}

Dialog::~Dialog()
{

}
