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
#include <QCheckBox>
#include "pattern.h"

int ptn_n;
pattern ptn[2];
int cnt1;
QCheckBox *pull_check;
QPointF MouseP;
int mouse_ind[2];
bool pull=1;
bool mouse_pull_push;
bool mouse_drop;
float my_scale=1.5;
int color_max=190;
float f;
int slider_circle_val;
int slider_weight_val;
int slider_current_val;
float test_val;
bool fire=0;
bool learning_yes;


QGroupBox *horizontalGroupBox ,
*horizontalGroupBox1, *horizontalGroupBox2, *horizontalGroupBox3;
QVBoxLayout *mainLayout, *pictureLayout;

QLineEdit *L_E, *L_E2, *L_E3, *L_E4, *L_E5;
QTimer *timer;
CNet net(10,0,IB);//4



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



myQPushButton *button1, *button_stop, *button_grab, *button_kill_delay,
*button_save_pattern, *button_learning;
myQSlider *slider_circle, *slider_show_ext,
*slider_weight_rad, *slider_current, *slider_freq,
*slider_weight_test, *slider_phase;
//QMenuBar* menuBar;
//work* WK;


void Dialog::startLearning()
{
    learning_yes=!learning_yes;
}

void Dialog::setMinWeight()
{
    net.minWeight=L_E->text().toFloat();
    net.normWeights();
    //    qDebug()<<net.minWeight;
}

void Dialog::setMaxWeight()
{
    net.maxWeight=L_E2->text().toFloat();
    net.kohonSettings();
    net.normWeights();
    //    qDebug()<<net.maxWeight;
}

void Dialog::setPattern(int i)
{
//    int i=0;
    for(int j=0;j<net.stim_ind.size();j++)
    {
        net.neuron[net.stim_ind[j]].external_I=0;
    }

    net.stim_ind=ptn[i].ind;
    for(int j=0;j<ptn[i].freq.size();j++)
        net.neuron[ptn[i].ind[j]].freq=ptn[i].freq[j];

    for(int j=0;j<net.stim_ind.size();j++)
    {
        net.neuron[net.stim_ind[j]].external_I=slider_current->value();
    }
}

void Dialog::savePattern()
{

    int i=ptn_n;
    ptn[i].freq.resize(net.stim_ind.size());
    ptn[i].ind=net.stim_ind;
    for(int j=0;j<net.stim_ind.size();j++)
        ptn[i].freq[j]=net.neuron[net.stim_ind[j]].freq;

    ptn_n++;
    ptn_n%=2;
}

void Dialog::changeWeight()
{

    int w;
    if(net.neuron[mouse_ind[1]].weight[mouse_ind[0]]>0.1)
        net.neuron[mouse_ind[1]].weight[mouse_ind[0]]=w=slider_weight_test->value();
    if(net.neuron[mouse_ind[1]].weight[mouse_ind[0]]<-0.1)
        net.neuron[mouse_ind[1]].weight[mouse_ind[0]]=w=-slider_weight_test->value();

    slider_weight_test->setToolTip("set weight: "+
                            QString::number(w));
}

void Dialog::setInhPerc()
{
    //    net=CNet(90,L_E3->text().toInt(),RS);
    for(int i=0;i<net.size;i++)
        net.neuron[i].is_excitatory=((rand()%100)>(L_E3->text().toInt()-1));
    net.weights_with_rad(slider_weight_rad->value());
}

void Dialog::killDelay()
{
    for(int i=0;i<net.size;i++)
        for(int j=0;j<net.size;j++)
            if(fabs(net.neuron[i].weight[j])>0.1)
                net.neuron[i].output[j].resize(3,0);
}

void Dialog::change_STDP_speed()
{
    net.STDP_speed=L_E4->text().toFloat();
}

void Dialog::currentChange(int x)
{
    slider_current_val=slider_current->value();
}

void Dialog::chooseThePattern()
{
    setPattern(L_E5->text().toInt());
}

Dialog::Dialog(QWidget *parent) :
    QDialog(parent)
{


    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawing()));

    timer->start(42);//24 times a sec

    button_stop= new myQPushButton(this,"stop spiking!");
    button1= new myQPushButton(this,"pull");
    button_grab= new myQPushButton(this,"grab");
    button_kill_delay=new myQPushButton(this,"kill delay");
    button_save_pattern=new myQPushButton(this,"save pattern");
    button_learning=new myQPushButton(this,"start learning");

    L_E=new QLineEdit;
    L_E2=new QLineEdit;
    L_E3=new QLineEdit;
    L_E4=new QLineEdit;
    L_E5=new QLineEdit;

    L_E->setText(QString::number(net.minWeight));
    L_E2->setText(QString::number( net.maxWeight));
    L_E3->setText(QString::number(0));
    L_E4->setText(QString::number(.01));
    L_E5->setText(QString::number(0));

    mainLayout = new QVBoxLayout();
    //    pictureLayout = new QVBoxLayout();

    QHBoxLayout *layout = new QHBoxLayout;
    QHBoxLayout *layout1 = new QHBoxLayout;
    QHBoxLayout *layout2 = new QHBoxLayout;
    QHBoxLayout *layout3 = new QHBoxLayout;
    horizontalGroupBox = new QGroupBox();
    horizontalGroupBox1 = new QGroupBox();
    horizontalGroupBox2 = new QGroupBox();
    horizontalGroupBox3 = new QGroupBox();
    //    QScrollArea *scroll = new QScrollArea;

    this->setGeometry(QRect(40,40,640,670));

    this->setLayout(mainLayout);

    horizontalGroupBox->setLayout(layout);
    horizontalGroupBox1->setLayout(layout1);
    horizontalGroupBox2->setLayout(layout2);
    horizontalGroupBox3->setLayout(layout3);


    //    horizontalGroupBox->setBaseSize(400,10);
    mainLayout->addWidget(horizontalGroupBox,100,Qt::AlignBottom);
    mainLayout->addWidget(horizontalGroupBox1,0,Qt::AlignBottom);
    mainLayout->addWidget(horizontalGroupBox2,0,Qt::AlignBottom);
    mainLayout->addWidget(horizontalGroupBox3,0,Qt::AlignBottom);
    //    mainLayout->addWidget(button,90,Qt::AlignBottom);
    //    mainLayout->addWidget(button1,1,Qt::AlignBottom);
    slider_weight_test= new myQSlider(this);
    slider_weight_test->setRange(1, 100);
    slider_weight_test->setValue(10);


    slider_show_ext = new myQSlider(this);
    slider_show_ext->setRange(10, 140);
    slider_show_ext->setValue(test_val=10);
    test_val/=20;

    //    test_val/=20;

    slider_circle = new myQSlider(this);
    slider_circle->setRange(1, 100);
    slider_circle->setValue(slider_circle_val=24);

    slider_weight_rad = new myQSlider(this);
    slider_weight_rad->setRange(8, 300);
    slider_weight_rad->setValue(net.weight_rad=slider_weight_val=50);

    slider_current = new myQSlider(this);
    //    slider_current->setRange(3000,6000);
    slider_current->setRange(5000,20000);
    slider_current = new myQSlider(this);
    slider_freq = new myQSlider(this);
    slider_freq->setRange(2,50);
    slider_freq->setValue(2);

    layout->addWidget(button_stop);
    layout->addWidget(button1);
    layout->addWidget(button_grab);
    layout->addWidget(slider_circle);
    layout->addWidget(slider_show_ext);
    layout1->addWidget(slider_weight_rad);
    layout1->addWidget(slider_current);
    layout1->addWidget(L_E);
    layout1->addWidget(L_E2);
    layout1->addWidget(L_E3);
    layout2->addWidget(slider_freq);
    layout2->addWidget(slider_weight_test);
    layout2->addWidget(L_E4);
    layout2->addWidget(button_kill_delay);
    layout2->addWidget(button_save_pattern);
    layout3->addWidget(button_learning);
    layout3->addWidget(L_E5);

    connect(button_learning,SIGNAL(clicked()),this,SLOT(startLearning()));

    connect(button_save_pattern,SIGNAL(clicked()),this,SLOT(savePattern()));

    connect(button_kill_delay,SIGNAL(clicked()),this,SLOT(killDelay()));

    connect(button1,SIGNAL(clicked()),this,SLOT(pull_change()));

    connect(button_grab,SIGNAL(clicked()),this,SLOT(neuroGrab()));

    connect(button_stop,SIGNAL(clicked()),this,SLOT(spikesStop()));

    connect(slider_show_ext, SIGNAL(valueChanged(int)), this,
            SLOT(trySlider2(int)));

    connect(slider_circle, SIGNAL(valueChanged(int)), this,
            SLOT(trySlider(int)));

    connect(slider_current, SIGNAL(valueChanged(int)), this,
            SLOT(currentChange(int)));

    connect(slider_freq, SIGNAL(sliderReleased()), this,
            SLOT(freqChange()));

    connect(slider_weight_rad,SIGNAL(sliderReleased()),this,SLOT(weightRadChanged()));

    connect(slider_weight_test,SIGNAL(sliderReleased()),this,SLOT(changeWeight()));

    connect(L_E,SIGNAL(editingFinished()),this,SLOT(setMinWeight()));

    connect(L_E2,SIGNAL(editingFinished()),this,SLOT(setMaxWeight()));

    connect(L_E3,SIGNAL(editingFinished()),this,SLOT(setInhPerc()));

    connect(L_E4, SIGNAL(editingFinished()), this,
            SLOT(change_STDP_speed()));

    connect(L_E5, SIGNAL(editingFinished()), this,
            SLOT(chooseThePattern()));



    slider_circle->setToolTip("subcicles, default is 24");
    slider_show_ext->setToolTip("speed of fake blinkings");
    slider_weight_rad->setToolTip("rad of weights");
    slider_current->setToolTip("set external current value");
    slider_freq->setToolTip("set modulator frequency: "+
                            QString::number(slider_freq->value()));
    slider_weight_test->setToolTip("set weight");
    L_E->setToolTip("set min weight");
    L_E2->setToolTip("set max weight");
    L_E3->setToolTip("inhibitory percentage");
    L_E4->setToolTip("STDP speed");
    L_E5->setToolTip("choose the pattern");

    freqChange();
    weightRadChanged();
    change_STDP_speed();
    this->currentChange(1);
    this->update();
    setPattern(1);

}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    if(event->text()==" ")
    {

        for(int i=0;i<net.stim_ind.size();i++)
        {
            net.neuron[net.stim_ind[i]].external_I=fire*slider_current_val;//40//2000

        }
        fire=!fire;

    }
    else if(event->text()=="h")
    {

        QString str;
        str+="neuron type: Izhikevich's neuron\n";
        str+="neuron subtype: ";
        str+=(net.type==RS)?"RS":"TC";
        str+="\nis excitatory: "+QString::number(net.neuron[mouse_ind[0]].is_excitatory);
        str+="\nID: "+QString::number(net.neuron[mouse_ind[0]].ID);
        str+="\nU_e: "+QString::number(net.neuron[mouse_ind[0]].U_e );
        str+="\nWeight: "+QString::number(net.neuron[mouse_ind[1]].weight[mouse_ind[0]] );
        str+="\nsynapse delay: "+QString::number(net.neuron[mouse_ind[1]].output[mouse_ind[0]].size() );
        str+="\ntest value: "+QString::number(net.STDP_speed);
        str+="\nfreq: "+QString::number(net.neuron[mouse_ind[0]].freq);
        str+="\nlearning is"+QString::number(learning_yes);
        str+="\n\n";

        this->setToolTip(str);

        std::cout<<str.toStdString();
        /*qDebug()<<net.STDP_speed;*/
        //        qDebug()<<str;
    }
    else if(event->text()=="i")
    {
        float w1;
        w1=fabs(net.neuron[mouse_ind[0]].weight[mouse_ind[1]]);
        net.neuron[mouse_ind[0]].weight[mouse_ind[1]] =fabs(net.neuron[mouse_ind[1]].weight[mouse_ind[0]]);
        net.neuron[mouse_ind[1]].weight[mouse_ind[0]]=w1;
        if(!net.neuron[mouse_ind[0]].is_excitatory)net.neuron[mouse_ind[0]].weight[mouse_ind[1]]*=-1;
        if(!net.neuron[mouse_ind[1]].is_excitatory)net.neuron[mouse_ind[1]].weight[mouse_ind[0]]*=-1;
        net.setArrows();
    }
    else if(event->text()=="s")
    {
        net.stim_ind.push_back(mouse_ind[0]);
        freqChange();
        //        QDebug
    }
    else if(event->text()=="m")
    {
        net.neuron[mouse_ind[1]].weight[mouse_ind[0]]=(net.maxWeight+net.minWeight)/2;
    }
    else if(event->text()=="l")
    {
        net.neuron[mouse_ind[1]].weight[mouse_ind[0]]=net.minWeight;
    }
    else if(event->text()=="r")
    {
        for(int i=0;i<net.stim_ind.size();i++)
        {
            if(net.stim_ind[i]==mouse_ind[0])
            {
                net.neuron[net.stim_ind[i]].external_I=0;
                net.stim_ind.erase(net.stim_ind.begin()+i);
            }
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

void Dialog::pull_change()
{
    pull=!pull;
    switch(pull)
    {
    case 0: button1->setText("puhing");break;
    case 1: button1->setText("pulling");

    }


}

void Dialog::spikesStop()
{
    net.spikesStop();
}

void Dialog::keyReleaseEvent(QKeyEvent *event)
{
    //    if(fire)
    //    {
    //        for(int i=0;i<net.stim_ind.size();i++)
    //        {
    //            net.neuron[net.stim_ind[i]].external_I=0;//40//2000
    //        }
    //        fire=0;
    //    }

}

void Dialog::freqChange()
{
    for(int i=0;i<net.stim_ind.size();i++)
    {
        net.neuron[mouse_ind[0]].freq=slider_freq->value();
        net.neuron[mouse_ind[0]].freq_cnt=0;
        net.neuron[mouse_ind[0]].time_from_freq=1000/slider_freq->value();
    }
    slider_freq->setToolTip("set modulator frequency: "+
                            QString::number(slider_freq->value()));
}

void Dialog::drawing()
{
    this->update();
}

void Dialog::neuroGrab()
{

    for(int i=0;i<net.size;i++)
        net.neuron[i].locate();

    net.kohonSettings();
    //    net.weights_with_rad(slider_weight_rad->value());
}

void Dialog::mouseReleaseEvent(QMouseEvent *e)
{
    mouse_drop=0;
    mouse_pull_push=0;
    QPointF V=MouseP-e->pos()/my_scale;
    if(QPointF::dotProduct(V,V)>net.rad*net.rad)
    {

        for(int j=0;j<net.size;j++)
        {
            net.setDelay(mouse_ind[0],j);
            net.setDelay(j,mouse_ind[0]);
        }
    }
    //    net.neuron[mouse_ind].external_I=0;
}

void Dialog::mouseMoveEvent(QMouseEvent *e)
{
    //    net.neuron[mouse_ind].vis=200;
    if(mouse_drop)
    {
        net.neuron[mouse_ind[0]].x=(e->x()/my_scale);
        net.neuron[mouse_ind[0]].y=(e->y()/my_scale);
        net.setArrows();
    }
    if(mouse_pull_push)
    {
        for(int i=0;i<net.size;i++)
            if(pull)
                net.neuron[i].pull(e->x()/my_scale,e->y()/my_scale);
            else net.neuron[i].push(e->x()/my_scale,e->y()/my_scale);
        net.setArrows();
    }

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
            mouse_ind[1]=mouse_ind[0];
            mouse_ind[0]=i;
            mouse_drop=1;
            //            net.neuron[i].vis=220;
        }
    }
    //
    if(mouse_drop==0)
        mouse_pull_push=1;

}

void Dialog::mainCircle()
{

    net.CalculateStep(test_val/slider_circle_val);

    //    net.


}

//rad=L_E_F->text().toFloat();

void Dialog::paintEvent(QPaintEvent* e)
{
    static int learn_cnt=0;
    learn_cnt++;
    if((learn_cnt>8)&&learning_yes)
    {
        learn_cnt=0;
        ptn_n++;
        ptn_n%=2;
        setPattern(ptn_n);
    }

    for (int i=0;i<slider_circle_val;i++)
        mainCircle();


    if(cnt1==6)
    {
        cnt1=0;
        net.normWeights();
    }
    cnt1++;

    QPainter* painter=new QPainter(this);
    //        painter->setRenderHint(QPainter::Antialiasing, 1);
    //    QPen pen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen pen(Qt::black);
    painter->setPen(pen);
    painter->scale(my_scale,my_scale);




    for(int i=0;i<net.size;i++)
        if((i!=mouse_ind[0]))
        {
            for(int j=0;j<net.size;j++)
            {
                float h=net.neuron[i].weight_norm[j]*color_max;

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
        float h=net.neuron[mouse_ind[0]].weight_norm[j]*color_max;
        if(h!=0)
        {
            if(!mouse_drop)
            {
                pen.setColor(QColor(h,h,h));
                painter->setPen(pen);
            }

            painter->drawLine(net.neuron[mouse_ind[0]].x,net.neuron[mouse_ind[0]].y,net.neuron[j].x,net.neuron[j].y);
        }
    }


    //    pen.setColor(QColor(0,0,0));
    pen=QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    for(int i=0;(i<net.size);i++)
        if((i!=mouse_ind[0]))
            for(int j=0;j<net.size;j++)
            {
                if(fabs(net.neuron[i].weight[j])>net.minWeight)
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
        if(fabs(net.neuron[mouse_ind[0]].weight[j])>net.minWeight)
        {
            painter->drawLine(net.neuron[j].x,net.neuron[j].y,
                              net.neuron[j].x+net.neuron[mouse_ind[0]].arrow[j].x[0],
                    net.neuron[j].y+net.neuron[mouse_ind[0]].arrow[j].y[0]);
            painter->drawLine(net.neuron[j].x,net.neuron[j].y,
                              net.neuron[j].x+net.neuron[mouse_ind[0]].arrow[j].x[1],
                    net.neuron[j].y+net.neuron[mouse_ind[0]].arrow[j].y[1]);
        }
    }




    for(int i=0;i<net.size;i++)
    {
        QPainterPath path;
        path.addEllipse ( QPointF(net.neuron[i].x,net.neuron[i].y),  net.rad,  net.rad );

        QRadialGradient gradient=QRadialGradient(QPointF(net.neuron[i].x,net.neuron[i].y),net.rad,
                                                 QPointF(net.neuron[i].x,net.neuron[i].y)+0.5*QPointF(net.rad,net.rad));


        if(net.neuron[i].is_excitatory)
        {
            gradient.setColorAt(1.0, QColor(net.neuron[i].vis,net.neuron[i].vis,net.neuron[i].vis));
            gradient.setColorAt(0.0, QColor(0,net.neuron[i].vis,0));
        }
        else
        {
            gradient.setColorAt(1.0, QColor(60,0.3*(200+net.neuron[i].vis),
                                            60));
            gradient.setColorAt(0.0, QColor(100,0.5*(200+net.neuron[i].vis),
                                            100));
        }

        painter->fillPath(path,gradient);


        for(int j=0;j<net.stim_ind.size();j++)
        {
            if(net.stim_ind[j]==i)
            {
                //     gradient(0, 0, 0, 100);
                if(net.neuron[i].is_excitatory)
                    gradient.setColorAt(1.0, QColor(0.5*(200+net.neuron[i].vis),(net.neuron[i].vis+110)*0.4,(220-net.neuron[i].vis)));

                else
                    gradient.setColorAt(1.0, QColor(0.5*(290+net.neuron[i].vis/1.5),0.3*(200+net.neuron[i].vis),
                                                    0.3*(200+net.neuron[i].vis)));

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

    //    net.weights_with_rad(slider_weight_rad->value());

}

//class myQHBoxLayout: public QHBoxLayout
//{

//};
