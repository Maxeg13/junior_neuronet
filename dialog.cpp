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
#include <vector>
#include <QLineEdit>



//#include "vars.h"
QPointF MouseP;
int mouse_ind;
bool mouse_drop;
float my_scale=1.5;

float f;
int slider_circle_val;
int slider_weight_val;
float test_val;

QLineEdit *L_E, *L_E2;
QTimer *timer;
CNet net(300,RS);//4



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

myQPushButton *button1, *button_stop;
myQSlider *slider_circle, *slider_show_ext, *slider_weight_rad;
QGroupBox* horizontalGroupBox , *horizontalGroupBox1;
QVBoxLayout *mainLayout, *pictureLayout;
//QMenuBar* menuBar;
//work* WK;


void Dialog::setMinWeight()
{
   net.minWeight=L_E->text().toFloat();
   qDebug()<<net.minWeight;
}

void Dialog::setMaxWeight()
{
   net.maxWeight=L_E2->text().toFloat();
   qDebug()<<net.maxWeight;
}

Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{

    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawing()));


    timer->start(40);

    button_stop= new myQPushButton(this,"stop spiking!");

    button1= new myQPushButton(this,"nothing button 1");

    L_E=new QLineEdit;

     L_E2=new QLineEdit;

    mainLayout = new QVBoxLayout();
    //    pictureLayout = new QVBoxLayout();

    QHBoxLayout *layout = new QHBoxLayout;
    QHBoxLayout *layout1 = new QHBoxLayout;
    horizontalGroupBox = new QGroupBox();
    horizontalGroupBox1 = new QGroupBox();
//    QScrollArea *scroll = new QScrollArea;

    this->setGeometry(QRect(40,40,650,600));

    this->setLayout(mainLayout);
    //    horizontalGroupBox->addWidget(button);
    horizontalGroupBox->setLayout(layout);
    horizontalGroupBox1->setLayout(layout1);


//    horizontalGroupBox->setBaseSize(400,10);
    mainLayout->addWidget(horizontalGroupBox,100,Qt::AlignBottom);
    mainLayout->addWidget(horizontalGroupBox1,0,Qt::AlignBottom);
    //    mainLayout->addWidget(button,90,Qt::AlignBottom);
    //    mainLayout->addWidget(button1,1,Qt::AlignBottom);

    slider_show_ext = new myQSlider(this);
    slider_show_ext->setRange(10, 90);
    slider_show_ext->setValue(test_val=10);
    test_val/=20;

    slider_circle = new myQSlider(this);
    slider_circle->setRange(1, 50);
    slider_circle->setValue(slider_circle_val=50);

    slider_weight_rad = new myQSlider(this);
    slider_weight_rad->setRange(8, 300);
    slider_weight_rad->setValue(net.weight_rad=slider_weight_val=50);

    layout->addWidget(button_stop);
    layout->addWidget(button1);
    layout->addWidget(slider_circle);
    layout->addWidget(slider_show_ext);
    layout1->addWidget(slider_weight_rad);
    layout1->addWidget(L_E);
        layout1->addWidget(L_E2);

    connect(button_stop,SIGNAL(clicked()),this,SLOT(spikesStop()));

    connect(slider_show_ext, SIGNAL(valueChanged(int)), this,
            SLOT(trySlider2(int)));

    connect(slider_circle, SIGNAL(valueChanged(int)), this,
            SLOT(trySlider(int)));

    connect(slider_weight_rad,SIGNAL(sliderReleased()),this,SLOT(weightRadChanged()));

    connect(L_E,SIGNAL(editingFinished()),this,SLOT(setMinWeight()));

     connect(L_E2,SIGNAL(editingFinished()),this,SLOT(setMaxWeight()));

slider_circle->setToolTip("subcicles, default is 50");
slider_show_ext->setToolTip("speed of fake blinkings");
slider_weight_rad->setToolTip("rad of weights");
L_E->setToolTip("set min weight");
L_E2->setToolTip("set max weight");
weightRadChanged();

    this->update();

}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    if(event->text()==" ")
    {
        for(int i=0;i<net.stim_ind.size();i++)
        {
            net.neuron[net.stim_ind[i]].external_I=2000;//40//2000
        }

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
    else if(event->text()=="s")
    {
        net.stim_ind.push_back(mouse_ind);
    }
    else if(event->text()=="r")
    {
        for(int i=0;i<net.stim_ind.size();i++)
        {
            if(net.stim_ind[i]==mouse_ind)
                net.stim_ind.erase(net.stim_ind.begin()+i);
        }
//        net.stim_ind.push_back(mouse_ind);
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

void Dialog::spikesStop()
{
    net.spikesStop();
}

void Dialog::keyReleaseEvent(QKeyEvent *event)
{
    for(int i=0;i<net.stim_ind.size();i++)
    {
        net.neuron[net.stim_ind[i]].external_I=0;//40//2000
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
    if(QPointF::dotProduct(V,V)>net.rad*net.rad)
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
        if(QPointF::dotProduct(V,V)<5*5)
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

//rad=L_E_F->text().toFloat();

void Dialog::paintEvent(QPaintEvent* e)
{
    for (int i=0;i<slider_circle_val;i++)
        mainCircle();

    QPainter* painter=new QPainter(this);
//    painter->setRenderHint(QPainter::Antialiasing, 1);
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
        path.addEllipse ( QPointF(net.neuron[i].x,net.neuron[i].y),  net.rad,  net.rad );
        QRadialGradient gradient=QRadialGradient(QPointF(net.neuron[i].x,net.neuron[i].y),net.rad,
                                                 QPointF(net.neuron[i].x,net.neuron[i].y)+0.5*QPointF(net.rad,net.rad));


            {
                //     gradient(0, 0, 0, 100);
                gradient.setColorAt(1.0, QColor(net.neuron[i].vis,net.neuron[i].vis,net.neuron[i].vis));
                gradient.setColorAt(0.0, QColor(0,net.neuron[i].vis,0));
                //    painter->fillPath(path,QBrush(QColor(0,0,0)));
                painter->fillPath(path,gradient);
            }

        for(int j=0;j<net.stim_ind.size();j++)
        {
            if(net.stim_ind[j]==i)
            {
                //     gradient(0, 0, 0, 100);
                gradient.setColorAt(1.0, QColor(net.neuron[i].vis,(net.neuron[i].vis+110)*0.4,(220-net.neuron[i].vis)));
                gradient.setColorAt(0.0, QColor(net.neuron[i].vis,0,100));
                //    painter->fillPath(path,QBrush(QColor(0,0,0)));
                painter->fillPath(path,gradient);

            }
        }

    }





    delete painter;
}

Dialog::~Dialog()
{

}

void Dialog::weightRadChanged()
{
//    net= CNet(24,RS);
    net.weights_with_rad(slider_weight_rad->value());
//    qDebug()<<slider_weight_rad->value();
//    qDebug()<<"hello";
}

//class myQHBoxLayout: public QHBoxLayout
//{

//};
