#include "dialog.h"
#include <QPainter>
#include <QTimer>
#include <QThread>
#include<QDebug>
//#include "work.h"


#include <QMouseEvent>
//#include "vars.h"
int nodes_N=340;
int lines_N=5;
float f;
QTimer *timer;
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

void Dialog::mousePressEvent(QMouseEvent *)
{

}

void Dialog::mainCircle()
{



}

void Dialog::paintEvent(QPaintEvent* e)
{
    static float t=1;
    t+=.06;
    if(t>10)t=10;
    for (int i=0;i<t;i++)
        mainCircle();

    QPainter* painter=new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing, 1);
    QPen pen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->scale(1.5,1.5);

//    for(int j=0;j<lines_N;j++)
//        painter->drawLine(ML[j].x[0],ML[j].y[0],ML[j].x[1],ML[j].y[1]);

//    pen=QPen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);


//    for(int j=0;j<nodes_N;j++)
//    {
//        pen.setColor(QColor(_node[j].clr[0],_node[j].clr[1],_node[j].clr[2]));
//        painter->setPen(pen);
//        painter->drawPoint(_node[j].x,_node[j].y);
//    }
    delete painter;
}

Dialog::~Dialog()
{

}
