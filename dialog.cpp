#include "dialog.h"
#include <QPainter>
#include <QTimer>
#include <QThread>
#include<QDebug>
#include "cnet.h"
#include "iostream"
#include <QMouseEvent>
#include <QVBoxLayout>
//#include <QMenuBar>
#include <QPushButton>
#include <QGroupBox>
#include <QSlider>




//#include "vars.h"
QPointF MouseP;
int mouse_ind;
bool mouse_drop;
float my_scale=1.5;
int rad=5;
float f;
int slider_circle_val=50;
float test_val;
QTimer *timer;
CNet net(30,RS);//4

QGroupBox* horizontalGroupBox;
QVBoxLayout *mainLayout, *pictureLayout;


class myQSlider:public QSlider
{
public:
    Dialog* d;
    myQSlider(Dialog* _d):QSlider(Qt::Horizontal, 0)
    {
        d=_d;
    }
    void keyPressEvent(QKeyEvent *event)
    {
        d->keyPressEvent(event);
    }
    void keyReleaseEvent(QKeyEvent *event)
    {
        d->keyReleaseEvent(event);
    } //
};


class myQPushButton:public QPushButton
{
public:
    Dialog* d;
    myQPushButton(Dialog* _d, QString s):QPushButton(s)
    {

        d=_d;
    }
    void keyPressEvent(QKeyEvent *event)
    {
        d->keyPressEvent(event);
    }
    void keyReleaseEvent(QKeyEvent *event)
    {
        d->keyReleaseEvent(event);
    } //
};

myQPushButton *button, *button1;
myQSlider *slider_circle, *slider_show_ext;
//QMenuBar* menuBar;
//work* WK;



Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{
    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawing()));


    timer->start(40);

    button= new myQPushButton(this,"nothing button");

    button1= new myQPushButton(this,"nothing button 1");
    mainLayout = new QVBoxLayout();
    //    pictureLayout = new QVBoxLayout();

    QHBoxLayout *layout = new QHBoxLayout;
    horizontalGroupBox = new QGroupBox();
//    QScrollArea *scroll = new QScrollArea;

    this->setGeometry(QRect(40,40,700,500));

    this->setLayout(mainLayout);
    //    horizontalGroupBox->addWidget(button);
    horizontalGroupBox->setLayout(layout);



//    horizontalGroupBox->setBaseSize(400,10);
    mainLayout->addWidget(horizontalGroupBox,100,Qt::AlignBottom);
    //    mainLayout->addWidget(button,90,Qt::AlignBottom);
    //    mainLayout->addWidget(button1,1,Qt::AlignBottom);

    slider_show_ext = new myQSlider(this);
    slider_show_ext->setRange(10, 50);
    slider_show_ext->setValue(test_val=10);
    test_val/=20;

    slider_circle = new myQSlider(this);
    slider_circle->setRange(1, 50);
    slider_circle->setValue(slider_circle_val=50);
//    QSlider::sliderReleased();

    layout->addWidget(button);
    layout->addWidget(button1);
    layout->addWidget(slider_circle);
    layout->addWidget(slider_show_ext);

    connect(slider_show_ext, SIGNAL(valueChanged(int)), this,
            SLOT(trySlider2(int)));

    connect(slider_circle, SIGNAL(valueChanged(int)), this,
            SLOT(trySlider(int)));
slider_circle->setToolTip("subcicles, default is 50");
slider_show_ext->setToolTip("speed of fake blinkings");

    this->update();

}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    if(event->text()==" ")
    {

        net.neuron[mouse_ind].external_I=2000;//40//2000
    }
    else if(event->text()=="h")
    {
        //        qDebug()<<"hello";
        QString str;
        str+="neuron type: Izhikevich's neuron\n";
        str+="neuron subtype: ";
        str+=(net.type==RS)?"RS\n":"TC\n";
        str+="ID: "+QString::number(net.neuron[mouse_ind].ID);
        str+="\n\n";
        this->setToolTip(str);

        std::cout<<str.toStdString();
    }
}

void Dialog::trySlider(int x)
{
//    qDebug()<<x;
    slider_circle_val=x;
}

void Dialog::trySlider2(int x)
{
    test_val=(float)x/20.;
//    qDebug()<<x;
//    net.ext_show=x/1000.;
}

void Dialog::keyReleaseEvent(QKeyEvent *event)
{
    if(event->text()==" ")
    {
        //qDebug()<<"hello";
        net.neuron[mouse_ind].external_I=0;//40//2000
    }
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

    //    this->setToolTipDuration(5000);
    //    this->setToolTipDuration(0);



    MouseP=(e->pos()/my_scale);//works in origin space
    QPointF V;
    for(int i=0;i<net.size;i++)
    {
        V=MouseP-QPointF(net.neuron[i].x,net.neuron[i].y);
        if(QPointF::dotProduct(V,V)<rad*rad)
        {
            mouse_ind=i;
            mouse_drop=1;
            //            net.neuron[i].vis=220;
        }
    }
    //

}

void Dialog::mainCircle()
{

    net.test(test_val/slider_circle_val);
    //    net.


}

void Dialog::paintEvent(QPaintEvent* e)
{
    for (int i=0;i<slider_circle_val;i++)
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


class myQHBoxLayout: public QHBoxLayout
{

};
