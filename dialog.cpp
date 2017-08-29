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
QPointF MouseP;
int mouse_ind;
bool mouse_drop;
float my_scale=1.5;
int rad=5;
float f;
QTimer *timer;
CNet net(35,RS);
//work* WK;



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
    mouse_drop=0;
    QPointF V=MouseP-e->pos()/my_scale;
    if(QPointF::dotProduct(V,V)>rad*rad)
    {
//        qDebug()<<"hello";
        for(int j=0;j<net.size;j++)
        {
        net.setDelay(mouse_ind,j);
        net.setDelay(j,mouse_ind);
        }
    }
    net.neuron[mouse_ind].external_I=0;
}

void Dialog::mouseMoveEvent(QMouseEvent *e)
{
//    net.neuron[mouse_ind].vis=200;
    if(mouse_drop)
    {
        net.neuron[mouse_ind].x=(e->x()/my_scale);
        net.neuron[mouse_ind].y=(e->y()/my_scale);
    }
    net.setArrows();
}

void Dialog::mousePressEvent(QMouseEvent *e)
{
    MouseP=(e->pos()/my_scale);//works in origin space
    QPointF V;
    for(int i=0;i<net.size;i++)
    {
        V=MouseP-QPointF(net.neuron[i].x,net.neuron[i].y);
        if(QPointF::dotProduct(V,V)<rad*rad)
        {
            mouse_ind=i;
            mouse_drop=1;
            net.neuron[i].vis=220;
        }
    }
    net.neuron[mouse_ind].external_I=2000;//40//2000

}

void Dialog::mainCircle()
{

    net.test();
//    net.


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




    for(int i=0;((i<net.size));i++)
        if((i!=mouse_ind)){
            for(int j=0;j<net.size;j++)
            {
                float h=net.neuron[i].weight_norm[j]*230;

                if(h!=0)
                {


                    pen.setColor(QColor(h,h,h));

                    painter->setPen(pen);
                    painter->drawLine(net.neuron[i].x,net.neuron[i].y,net.neuron[j].x,net.neuron[j].y);
                }
            }
        }

    pen.setColor(QColor(150,0,0));
    painter->setPen(pen);
    for(int j=0;j<net.size;j++)
    {
        float h=net.neuron[mouse_ind].weight_norm[j]*250;
        if(h!=0)
        {
            if(!mouse_drop)
            {
                pen.setColor(QColor(h,h,h));
                painter->setPen(pen);
            }

            painter->drawLine(net.neuron[mouse_ind].x,net.neuron[mouse_ind].y,net.neuron[j].x,net.neuron[j].y);
        }
    }


    //    pen.setColor(QColor(0,0,0));
    pen=QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    for(int i=0;(i<net.size);i++)
        if((i!=mouse_ind))
        for(int j=0;j<net.size;j++)
        {
            if(net.neuron[i].weight[j]!=0)
            {

                painter->drawLine(net.neuron[j].x,net.neuron[j].y,
                                  net.neuron[j].x+net.neuron[i].arrow[j].x[0],
                        net.neuron[j].y+net.neuron[i].arrow[j].y[0]);
                painter->drawLine(net.neuron[j].x,net.neuron[j].y,
                                  net.neuron[j].x+net.neuron[i].arrow[j].x[1],
                        net.neuron[j].y+net.neuron[i].arrow[j].y[1]);


            }
        }

    //mouse ,arrows
    if(mouse_drop)
    {
        pen.setColor(QColor(150,0,0));
    }else
        pen.setColor(QColor(0,0,0));
    painter->setPen(pen);

    for(int j=0;j<net.size;j++)
    {
        if(net.neuron[mouse_ind].weight[j]!=0)
        {
            painter->drawLine(net.neuron[j].x,net.neuron[j].y,
                              net.neuron[j].x+net.neuron[mouse_ind].arrow[j].x[0],
                    net.neuron[j].y+net.neuron[mouse_ind].arrow[j].y[0]);
            painter->drawLine(net.neuron[j].x,net.neuron[j].y,
                              net.neuron[j].x+net.neuron[mouse_ind].arrow[j].x[1],
                    net.neuron[j].y+net.neuron[mouse_ind].arrow[j].y[1]);
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
                                                 QPointF(net.neuron[i].x,net.neuron[i].y)+0.5*QPointF(rad,rad));
        //     gradient(0, 0, 0, 100);
        gradient.setColorAt(1.0, QColor(net.neuron[i].vis,net.neuron[i].vis,net.neuron[i].vis));
        gradient.setColorAt(0.0, QColor(0,net.neuron[i].vis,0));
        //    painter->fillPath(path,QBrush(QColor(0,0,0)));
        painter->fillPath(path,gradient);
    }


    delete painter;
}

Dialog::~Dialog()
{

}
