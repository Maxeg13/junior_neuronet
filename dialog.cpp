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
#include "datacollector.h"
DataCollector* collector;
//QPixmap pmap;
bool drawing_on=1;
QwtPlotCurve rastrCurve, weightCurve;
QwtPlot *plot_rastr, *plot_weights;
QColor QCLR;
QPolygon qp;
QVector<QPoint> qpt;
QColor *dropColor1, *dropColor2;
QColor inhibColor=QColor(100,25,25);
QColor QCLR_red_spike=QColor(250,100,0,255);
QColor QCLR_blue_spike=QColor(0,100,220);
int transp_val=120;
int ptn_n=0;
pattern ptn[2];
int cnt1;
QCheckBox *pull_check;
QPointF MouseP;
int mouse_ind[2];
bool pull=1;
bool mouse_pull_push;
bool mouse_drop;
float my_scale;
int syn_color_max=120;
float f;
int slider_circle_val;
int slider_weight_val;
int slider_current_val;
float test_val;
bool fire=0;
bool learning_yes;
bool draw_links=1;
bool redraw=0;
int g_size=2600;


QLineEdit *L_E, *L_E2, *L_E3, *L_E4, *L_E5;
QTimer *timer;
CNet net(4,4,0,RS);
//CNet net(225,225,0,RS);
//18 IB Kohonen CNet net(18,0,IB);
//CNet net(50,0,RS);//for demo

void Dialog::getEMG(vector<float> x )
{

}

void drawLinkWithSpike(int, int , QColor& ,QColor&, QPen& ,QPainter* );

int colorThresh(int x)
{
    if(x>(syn_color_max+20))
    {
        return((syn_color_max+20));
    }
    else
        return(x);

}

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

//class myLE:public QLineEdit
//{

//}


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
*button_save_pattern, *button_learning, *button_poisson, *button_drawing;
QSlider *slider_circle, *slider_show_ext,
*slider_weight_rad, *slider_current, *slider_freq,
*slider_weight_test, *slider_phase, *slider_scale,
*slider_pois_time, *slider_depression_alpha,
*slider_w_probab, *slider_inh_k, *slider_stim_weight;
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
}



void Dialog::setMaxWeight()
{
    net.maxWeight=L_E2->text().toFloat();
    if(net.demo)
        net.demoSettings(slider_weight_rad->value());
    else
        net.LobovSettings();

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
    {
        //        net.neuron[ptn[i].ind[j]].freq_cnt=0;
        net.neuron[ptn[i].ind[j]].freq=ptn[i].freq[j];
        net.neuron[ptn[i].ind[j]].time_from_freq=1000/ptn[i].freq[j];
    }

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

//    int i=1;
//    ptn[i].freq.resize(net.stim_ind.size());
//    ptn[i].ind=net.stim_ind;
//    for(int j=0;j<ptn[0].ind.size();j++)
//    {
//        //            qDebug()<<"hello";
//        //        ptn[1].ind[j]=ptn[0].ind[net.stim_ind.size()-1-j];
//        ptn[i].freq[j]=ptn[0].freq[j];

//    }

//    i=0;
//    ptn[i].freq.resize(net.stim_ind.size());
//    ptn[i].ind=net.stim_ind;
//    for(int j=0;j<net.stim_ind.size();j++)
//        ptn[i].freq[j]=net.neuron[net.stim_ind[j]].freq;


}

void Dialog::setPhase()
{
    net.neuron[mouse_ind[0]].freq_phase=slider_phase->value();
}

void Dialog::changePoisInterval()
{
    net.poisson_interval=slider_pois_time->value();
    net.T_eff=net.poisson_interval*1.9/log(2.718);
    slider_pois_time->setToolTip(QString("poisson interv=")+QString::number(net.poisson_interval)+QString(" ms"));
}

void Dialog::changeWeight()
{

    float w;
    //    if(net.neuron[mouse_ind[1]].weight[mouse_ind[0]]>0.00001)
    //    net.neuron[mouse_ind[1]].weight[mouse_ind[0]]=w=slider_weight_test->value();
    w=net.maxWeight=slider_weight_test->value();
    net.LobovSettings();
//    weightRadChanged();


    slider_weight_test->setToolTip("set weight: "+
                                   QString::number(w));
}

void Dialog::setInhPerc()
{
    //    net=CNet(90,L_E3->text().toInt(),RS);
    for(int i=0;i<net.size;i++)
        net.neuron[i].is_excitatory=((rand()%100)>(L_E3->text().toInt()-1));
   net.weightsWithRad(slider_weight_rad->value(),slider_stim_weight->value()/10.,slider_w_probab->value());
}

void Dialog::changePoisson()
{
    net.poisson_on=!net.poisson_on;
    //    qDebug()<<(int)
    switch(net.poisson_on)
    {
    case false:
        button_poisson->setText("poisson off");break;
    case true:
        button_poisson->setText("poisson on");break;
    }
    //    button_poisson->setText("");
}

void Dialog::killDelay()
{
    for(int i=net.detectors_size;i<net.size;i++)
        for(int j=0;j<net.size;j++)
            if(fabs(net.neuron[i].weight[j])>0.00001)
                net.neuron[i].eff_dist[j]=0;
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

    pmap=QPixmap(g_size,g_size);
    QPainter pr(&pmap);
    QBrush br(QColor(240,240,240));
    QPen pn(QColor(240,240,240));
    pr.setBrush(br);
    pr.setPen(pn);
    pr.drawRect(QRect(0,0,g_size,g_size));
    //        pmap

    plot_rastr = new QwtPlot();
    drawingInit(plot_rastr,QString("Rastr"));
    plot_rastr->setAxisScale(QwtPlot::yLeft,0,net.size);
    plot_rastr->setAxisScale(QwtPlot::xBottom,0,2000);
    plot_rastr->setAxisTitle(QwtPlot::yLeft, "neuron's index");
    plot_rastr->setAxisTitle(QwtPlot::xBottom, "time");
    //    plot_rastr->show();


    plot_weights = new QwtPlot();
    drawingInit(plot_weights,QString("weights distribution"));
    plot_weights->setAxisScale(QwtPlot::yLeft,0,net.maxWeight*1.6);
    plot_weights->setAxisScale(QwtPlot::xBottom,0,net.size);
    plot_weights->setAxisTitle(QwtPlot::yLeft, "weight");
    plot_weights->setAxisTitle(QwtPlot::xBottom, "neuron's index");
    //    plot_weights->show();

    QWidget* MW;
    MW=new QWidget();
    MW->show();
    QGridLayout* QGL2=new QGridLayout(MW);
    QGL2->addWidget(plot_weights,0,0,1,1);
    QGL2->addWidget(plot_rastr,1,0,1,1);

    qpt.push_front(QPoint(10,10));
    qpt.push_front(QPoint(100,10));
    qpt.push_front(QPoint(100,14));
    qpt.push_front(QPoint(10,14));
    qp=QPolygon(qpt);


    dropColor1=new QColor(150,0,0);
    dropColor2=new QColor(0,0,150);

    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawing()));

    timer->start(42);//24 times a sec

    button_drawing=new myQPushButton(this,"drawing on");
    button_poisson=new myQPushButton(this,"poisson on");
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
    L_E4->setText(QString::number(25));//STDP speed
    L_E5->setText(QString::number(0));

    QGridLayout* QGL=new QGridLayout(this);
    //    mainLayout = new QVBoxLayout();


    this->setGeometry(QRect(40,40,1200,1000));

    slider_stim_weight=new myQSlider(this);
    slider_stim_weight->setRange(0,10);
    slider_stim_weight->setValue(5);
    slider_stim_weight->setOrientation(Qt::Horizontal);

    slider_pois_time=new myQSlider(this);
    slider_pois_time->setRange(100, 700);
    slider_pois_time->setValue(100);
    slider_pois_time->setOrientation(Qt::Horizontal);

    slider_scale= new myQSlider(this);
    slider_scale->setRange(4, 20);
    slider_scale->setValue(15);
    slider_scale->setOrientation(Qt::Horizontal);

    slider_weight_test= new myQSlider(this);
    slider_weight_test->setRange(0, 20);
    slider_weight_test->setValue(12);
    slider_weight_test->setOrientation(Qt::Horizontal);

    slider_show_ext = new myQSlider(this);
    slider_show_ext->setRange(10, 140);
    slider_show_ext->setValue(test_val=99);
    slider_show_ext->setOrientation(Qt::Horizontal);
    test_val/=20;


    slider_depression_alpha = new myQSlider(this);
    slider_depression_alpha->setRange(0, 15);
    slider_depression_alpha->setValue(14);//5
    slider_depression_alpha->setOrientation(Qt::Horizontal);
    slider_depression_alpha->setToolTip(QString("depression eff"));

    //    test_val/=20;

    slider_circle = new QSlider(this);
    slider_circle->setRange(1, 60);
    slider_circle->setValue(slider_circle_val=24);
    slider_circle->setOrientation(Qt::Horizontal);
    slider_weight_rad = new myQSlider(this);
    slider_weight_rad->setRange(8, 300);
    slider_weight_rad->setValue(net.weight_rad=slider_weight_val=121);
    slider_weight_rad->setOrientation(Qt::Horizontal);
    slider_current = new myQSlider(this);
    slider_current->setRange(1,50);
    slider_current-> setValue(50);
    slider_current->setOrientation(Qt::Horizontal);
    slider_phase= new myQSlider(this);
    slider_phase->setRange(1,100);
    slider_phase->setValue(1);
    slider_phase->setOrientation(Qt::Horizontal);
    slider_freq = new myQSlider(this);
    slider_freq->setRange(2,50);
    slider_freq->setValue(8);
    slider_freq->setOrientation(Qt::Horizontal);
    slider_w_probab = new myQSlider(this);
    slider_w_probab->setRange(1,5);
    slider_w_probab->setValue(3);
    slider_w_probab->setOrientation(Qt::Horizontal);
    slider_inh_k = new myQSlider(this);
    slider_inh_k->setRange(0,10);
    slider_inh_k->setValue(5);
    slider_inh_k->setOrientation(Qt::Horizontal);

    QGL->addWidget(button_stop,0,0,1,1,Qt::AlignBottom);
    QGL->addWidget(button1,0,1,1,1,Qt::AlignBottom);
    QGL->addWidget(button_grab,0,2,1,1,Qt::AlignBottom);
    QGL->addWidget(slider_circle,0,3,1,1,Qt::AlignBottom);
    QGL->addWidget(slider_show_ext,0,4,1,1,Qt::AlignBottom);

    QGL->addWidget(slider_weight_rad,1,0,1,1);
    QGL->addWidget(slider_w_probab,1,1,1,1);
    QGL->addWidget(L_E,1,2,1,1);
    QGL->addWidget(L_E2,1,3,1,1);
    QGL->addWidget(L_E3,1,4,1,1);

    QGL->addWidget(slider_freq,2,0,1,1);
    QGL->addWidget(slider_weight_test,2,1,1,1);
    QGL->addWidget(L_E4,2,2,1,1);
    QGL->addWidget(button_kill_delay,2,3,1,1);
    QGL->addWidget(button_save_pattern,2,4,1,1);

    QGL->addWidget(button_learning,3,0,1,1);
    QGL->addWidget(L_E5,3,1,1,1);
    QGL->addWidget(slider_scale,3,2,1,1);
    QGL->addWidget(slider_phase,3,3,1,1);
    QGL->addWidget(button_poisson,3,4,1,1);

    QGL->addWidget(slider_pois_time,4,0,1,1);
    QGL->addWidget(slider_depression_alpha,4,1,1,1);
    QGL->addWidget(button_drawing,4,2,1,1);
    QGL->addWidget(slider_current,4,3,1,1);
    QGL->addWidget(slider_inh_k,4,4,1,1);
    QGL->addWidget(slider_stim_weight,5,0,1,1);



    connect(button_drawing,SIGNAL(clicked()),this,SLOT(changeDrawing()));

    connect(button_learning,SIGNAL(clicked()),this,SLOT(startLearning()));

    connect(button_poisson,SIGNAL(clicked()),this,SLOT(changePoisson()));

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

    connect(slider_inh_k,SIGNAL(sliderReleased()),this,SLOT(weightRadChanged()));
    connect(slider_weight_rad,SIGNAL(sliderReleased()),this,SLOT(weightRadChanged()));
    connect(slider_w_probab,SIGNAL(sliderReleased()),this,SLOT(weightRadChanged()));

    connect(slider_weight_test,SIGNAL(sliderReleased()),this,SLOT(changeWeight()));

    connect(slider_stim_weight,SIGNAL(sliderReleased()),this,SLOT(changeWeight()));

    connect(slider_pois_time,SIGNAL(sliderReleased()),this,SLOT(changePoisInterval()));

    connect(L_E,SIGNAL(returnPressed()),this,SLOT(setMinWeight()));

    connect(L_E2,SIGNAL(returnPressed()),this,SLOT(setMaxWeight()));

    connect(L_E3,SIGNAL(returnPressed()),this,SLOT(setInhPerc()));

    connect(L_E4, SIGNAL(returnPressed()), this,
            SLOT(change_STDP_speed()));

    connect(L_E5, SIGNAL(returnPressed()), this,
            SLOT(chooseThePattern()));

    connect(slider_phase, SIGNAL(sliderReleased()),this,SLOT(setPhase()));

    //    connect(&(collector->qdc),SIGNAL(EMG( vector<float>)),this,SLOT(getEMG( vector<float>)));

    slider_scale->setToolTip("set picture size");
    slider_circle->setToolTip("subcicles, default is 24");
    slider_show_ext->setToolTip("speed of fake blinkings");
    slider_weight_rad->setToolTip("rad of weights");
    slider_current->setToolTip("set external current value");
    slider_freq->setToolTip("set modulator frequency: "+
                            QString::number(slider_freq->value()));
    slider_weight_test->setToolTip("set weight");
    slider_phase->setToolTip("set phase");
    slider_w_probab->setToolTip("linking probab");
    slider_inh_k->setToolTip("inhibitory efficiency");
    slider_stim_weight->setToolTip("koefficient of stim weights");

    L_E->setToolTip("set min weight");
    L_E2->setToolTip("set max weight");
    L_E3->setToolTip("inhibitory percentage");
    L_E4->setToolTip("STDP speed");
    L_E5->setToolTip("choose the pattern");

    //    net.kohonSettings();
    changePoisInterval();
//    weightRadChanged();
    freqChange();
    change_STDP_speed();
    this->currentChange(1);
    //    mouse_drop=1;
    this->update();
    setPattern(1);

}

void Dialog::keyPressEvent(QKeyEvent *event)
{
    if(event->text()==" ")
    {

        for(int i=0;i<net.stim_ind.size();i++)
        {
            net.neuron[net.stim_ind[i]].freq_cnt=0;
            net.neuron[net.stim_ind[i]].external_I=fire*slider_current_val;//40//2000

        }
        fire=!fire;

    }
    else if(event->text()=="h")
    {

        QString str;
        str+="neuron type: Izhikevich's neuron\n";
        str+="neuron subtype: ";
        str+=(net.type==RS)?"RS":(net.type==TC)?"TC":"IB";
        str+="\nis excitatory: "+QString::number(net.neuron[mouse_ind[0]].is_excitatory);
        str+="\nID: "+QString::number(net.neuron[mouse_ind[0]].ID);
        str+="\nE_m: "+QString::number(net.neuron[mouse_ind[0]].E_m );
        str+="\nU_e: "+QString::number(net.neuron[mouse_ind[0]].U_e );
        str+="\nWeight: "+QString::number(net.neuron[mouse_ind[1]].weight[mouse_ind[0]] );
        str+="\nsynapse delay: "+QString::number(net.neuron[mouse_ind[1]].eff_dist[mouse_ind[0]] );
        str+="\ntest value: "+QString::number(net.STDP_speed);
        str+="\nfreq: "+QString::number(net.neuron[mouse_ind[0]].freq);
        str+="\nlearning is: "+QString::number(learning_yes);
        str+="\ntime(freq): "+QString::number(net.neuron[mouse_ind[0]].time_from_freq);
        str+="\nactiv: "+QString::number(net.neuron[mouse_ind[0]].activity);
        str+="\n\n";

        this->setToolTip(str);

        std::cout<<str.toStdString();
        /*qDebug()<<net.STDP_speed;*/
        //                qDebug()<<str;
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
    }
    else if(event->text()=="m")
    {
        net.neuron[mouse_ind[1]].weight[mouse_ind[0]]=
                ((net.neuron[mouse_ind[1]].weight[mouse_ind[0]]>
                .0001)?(1):(net.neuron[mouse_ind[1]].weight[mouse_ind[0]]<-.0001)?(-1):
                                                                                  (0))*(net.maxWeight+net.minWeight)/2;
    }
    else if(event->text()=="l")
    {
        net.neuron[mouse_ind[1]].weight[mouse_ind[0]]=net.maxWeight;
        //                net.
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
    else if(event->text()=="d")
    {
        for(int i=0;i<net.stim_ind.size();i++)
            net.neuron[net.stim_ind[i]].external_I=0;
        net.stim_ind.resize(0);
    }
    else if(event->text()=="k")
    {
        //       for(int i=0;i<net.size;i++)
        for (int j=0;j<net.size;j++)
        {
            net.neuron[mouse_ind[0]].weight[j]=0;
            net.neuron[j].weight[mouse_ind[0]]=0;
            net.neuron[mouse_ind[0]].with_poisson=0;
        }
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
    //    for(int i=0;i<net.stim_ind.size();i++)
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

    if(net.demo)
        net.demoSettings(slider_weight_rad->value());
    else
        net.LobovSettings();
    //    net.weightsWithRad(slider_weight_rad->value());
}

void Dialog::mouseReleaseEvent(QMouseEvent *e)
{
    mouse_drop=0;
    redraw=0;
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
        if(QPointF::dotProduct(V,V)<net.rad*net.rad)
        {
            mouse_ind[1]=mouse_ind[0];
            mouse_ind[0]=i;
            mouse_drop=1;
            redraw=1;
            //            qDebug()<<net.neuron[i].top[0];
            net.neuron[i].rangedRemoted();
            //            net.neuron[i].vis=220;
        }
    }
    //
    //    if(mouse_drop==0)
    //        mouse_pull_push=1;

}

void Dialog::mainCircle()
{

    net.CalculateStep(test_val/slider_circle_val);

    //    net.


}

//rad=L_E_F->text().toFloat();

void Dialog::paintEvent(QPaintEvent* e)
{







    net.alpha=slider_depression_alpha->value()/10.;
    QwtSymbol* symbol = new QwtSymbol( QwtSymbol::Ellipse,
                                       QBrush(QColor(0,0,0)), QPen(QColor(0,0,0,0)  ), QSize( 5, 5) );
    rastrCurve.setSymbol(symbol);
    rastrCurve.setPen(QColor(0,0,0,0));
    QPolygonF points;
    for(int i=0;i<net.size;i++)
        for(int j=0;j<net.neuron[i].rastr.size();j++)
            points<<QPointF(net.neuron[i].rastr[j],i);

    rastrCurve.setSamples( points ); // ассоциировать набор точек с кривой
    rastrCurve.attach( plot_rastr); // отобразить кривую на графике/



    //    QwtSymbol* symbol = new QwtSymbol( QwtSymbol::Ellipse,
    //                                       QBrush(QColor(0,0,0)), QPen(QColor(0,0,0,0)  ), QSize( 5, 5) );
    //    weightCurve.setSymbol(symbol);
    weightCurve.setPen(QColor(0,0,255));
    //    QPolygonF points;
    points.resize(0);
    //    for(int i=0;i<net.size;i++)
    for(int j=0;j<net.weight_ind.size();j++)
        points<<QPointF(j,net.neuron[mouse_ind[0]].weight[net.weight_ind[j]]);
    //            points<<QPointF(j,2);

    weightCurve.setSamples( points ); // ассоциировать набор точек с кривой
    weightCurve.attach( plot_weights); // отобразить кривую на графике/
//    plot_weights->setAxisScale(QwtPlot::yLeft,0,net.maxWeight);
 plot_weights->setAxisScale(QwtPlot::yLeft,0,net.maxWeight*1.6);

    //    emit L_E->editingFinished();
    static float blink_phase=0;
    blink_phase++;
    //    QCLR_red_spike=QColor(250,100*0.5*(1+sin(0.9*blink_phase)),100*0.5*(1+sin(0.9*blink_phase)),255);
    //    QCLR_blue_spike=QColor(120*0.5*(1+sin(0.9*blink_phase)),120*0.5*(1+sin(0.9*blink_phase)),220*0.5*(1-sin(0.9*blink_phase)));

    static int learn_cnt=0;
    learn_cnt++;
    if((learn_cnt>30)&&learning_yes)
    {
        learn_cnt=0;
        ptn_n++;
        ptn_n%=2;
        setPattern(ptn_n);
        L_E5->setText(QString::number(ptn_n));
    }

    for (int i=0;i<slider_circle_val;i++)
        mainCircle();







    QPainter* painter_basic=new QPainter(this);
    QPainter* painter=new QPainter(&pmap);
    painter_basic->setRenderHint(QPainter::Antialiasing, 1);
    painter->setRenderHint(QPainter::Antialiasing, 1);

    //    QPen pen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QPen pen(Qt::red);

    //    painter->scale(my_scale,my_scale);
    //    painter->scale(my_scale=(slider_scale->value()/10.), slider_scale->value()/10.);
    painter_basic->scale(my_scale=(slider_scale->value()/10.), slider_scale->value()/10.);


    // DRAWING THE NET!!!!


    if(drawing_on)
    {
        if(redraw||mouse_drop)//erase with rect
        {
            QBrush br(QColor(240,240,240));
            QPen pn(QColor(240,240,240));
            painter->setBrush(br);
            painter->setPen(pn);
            painter->drawRect(QRect(0,0,g_size,g_size));

        }
        if(cnt1==6)
        {
            cnt1=0;
            net.normWeights();
        }
        cnt1++;



        pen.setColor(Qt::black);
        pen.setWidth(2);
        //        qDebug()<<"redr="<<redraw<<"  mouse_drop="<<mouse_drop;


        if(redraw||mouse_drop)
            if(draw_links)
                for(int i=0;i<net.size;i++)
                    //                    if((i!=mouse_ind[0]))
                {
                    for(int j=0;j<net.size;j++)
                    {
                        int h=colorThresh(net.neuron[i].weight_norm[j]*syn_color_max);
                        QCLR=QColor(h,h,h,transp_val);

                        if((net.neuron[i].weight[j])>0.0001)
                        {
                            if(net.spike_show)
                            {
                                //                            QCLR=QColor(h,h,h, transp_val);
                                QCLR.setBlue(h);
                                QCLR.setGreen(h);
                                QCLR.setRed(h);
                                QCLR.setAlpha(transp_val);
                                //                        pen.setColor(QColor(h,h,h));
                                drawLinkWithSpike(i,j,QCLR,QCLR_blue_spike, pen,painter);
                            }
                            else
                                //                            if(net.neuron[i].syn_cnt[j].size())
                            {
                                QCLR.setBlue(h);
                                QCLR.setGreen(h);
                                QCLR.setRed(h);
                                QCLR.setAlpha(255);
                                pen.setColor(QCLR);
                                painter->setPen(pen);
                                painter->drawLine(net.neuron[i].x,net.neuron[i].y,net.neuron[j].x,net.neuron[j].y);
                            }
                        }
                        else if((net.neuron[i].weight[j])<-0.0001)
                        {
                            QCLR.setBlue(100);
                            QCLR.setGreen(100);
                            QCLR.setRed(140);
                            QCLR.setAlpha(transp_val);

                            if(net.spike_show)
                            {
                                drawLinkWithSpike(i,j,QCLR,QCLR_red_spike, pen,painter);
                            }
                            else
                                //                            if(net.neuron[i].syn_cnt[j].size())
                            {
                                pen.setColor(QCLR);
                                painter->setPen(pen);
                                painter->drawLine(net.neuron[i].x,net.neuron[i].y,net.neuron[j].x,net.neuron[j].y);
                            }

                        }
                    }
                }

        painter->setPen(pen);
//        for(int j=0;j<net.size;j++)
//        {
//            float h=colorThresh(net.neuron[mouse_ind[0]].weight_norm[j]*syn_color_max);

            if(mouse_drop||redraw)
            {
                for(int j=0;j<net.neuron[mouse_ind[0]].link_plus_ind.size();j++)
                {
//                    float h=colorThresh(net.neuron[mouse_ind[0]].weight_norm[link_plus_ind[j]]*syn_color_max);
                    pen.setWidth(3);
                    pen.setColor(*dropColor2);
                    painter->setPen(pen);
                    painter->drawLine(net.neuron[mouse_ind[0]].x,net.neuron[mouse_ind[0]].y,net.neuron[net.neuron[mouse_ind[0]].link_plus_ind[j]].x,net.neuron[net.neuron[mouse_ind[0]].link_plus_ind[j]].y);
                    pen.setWidth(2);//2
                }
                for(int j=0;j<net.neuron[mouse_ind[0]].link_min_ind.size();j++)
                {
//                    float h=colorThresh(net.neuron[mouse_ind[0]].weight_norm[link_min_ind[j]]*syn_color_max);
                    pen.setWidth(3);
                    pen.setColor(*dropColor1);
                    painter->setPen(pen);
                    painter->drawLine(net.neuron[mouse_ind[0]].x,net.neuron[mouse_ind[0]].y,net.neuron[net.neuron[mouse_ind[0]].link_min_ind[j]].x,net.neuron[net.neuron[mouse_ind[0]].link_min_ind[j]].y);
                    pen.setWidth(2);
                }

            }


//        }




        //    pen.setColor(QColor(0,0,0));

        if(mouse_drop||redraw)
            if(draw_links)//arrows
            {
                pen=QPen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
                painter->setPen(pen);
                for(int i=0;(i<net.size);i++)
                    if((i!=mouse_ind[0]))
                        for(int j=0;j<net.size;j++)
                        {
                            if(fabs(net.neuron[i].weight[j])>.0001)
                            {
                                //                            if(net.neuron[i].syn_cnt[j].size())
                                {
                                    painter->drawLine(net.neuron[j].x,net.neuron[j].y,
                                                      net.neuron[j].x+net.neuron[i].arrow[j].x[0],
                                            net.neuron[j].y+net.neuron[i].arrow[j].y[0]);
                                    painter->drawLine(net.neuron[j].x,net.neuron[j].y,
                                                      net.neuron[j].x+net.neuron[i].arrow[j].x[1],
                                            net.neuron[j].y+net.neuron[i].arrow[j].y[1]);
                                }

                            }
                        }
            }



        if(redraw||mouse_drop) {
            for(int j=0;j<net.size;j++)
            {
                if( (net.neuron[mouse_ind[0]].weight[j])<-0.0001)
                {
                    //                pen.setWidth(3);
                    pen.setColor(*dropColor1);
                    painter->setPen(pen);
                    painter->drawLine(net.neuron[j].x,net.neuron[j].y,
                                      net.neuron[j].x+net.neuron[mouse_ind[0]].arrow[j].x[0],
                            net.neuron[j].y+net.neuron[mouse_ind[0]].arrow[j].y[0]);
                    painter->drawLine(net.neuron[j].x,net.neuron[j].y,
                                      net.neuron[j].x+net.neuron[mouse_ind[0]].arrow[j].x[1],
                            net.neuron[j].y+net.neuron[mouse_ind[0]].arrow[j].y[1]);
                }
                else if( (net.neuron[mouse_ind[0]].weight[j])>0.0001)
                {
                    //                pen.setWidth(3);
                    pen.setColor(*dropColor2);
                    painter->setPen(pen);
                    painter->drawLine(net.neuron[j].x,net.neuron[j].y,
                                      net.neuron[j].x+net.neuron[mouse_ind[0]].arrow[j].x[0],
                            net.neuron[j].y+net.neuron[mouse_ind[0]].arrow[j].y[0]);
                    painter->drawLine(net.neuron[j].x,net.neuron[j].y,
                                      net.neuron[j].x+net.neuron[mouse_ind[0]].arrow[j].x[1],
                            net.neuron[j].y+net.neuron[mouse_ind[0]].arrow[j].y[1]);
                }


            }
        }




        painter_basic->drawPixmap(0,0,pmap);




        for(int i=0;i<net.size;i++)
        {
            QPainterPath path;
            path.addEllipse ( QPointF(net.neuron[i].x,net.neuron[i].y),  net.rad,  net.rad );

            QRadialGradient gradient=QRadialGradient(QPointF(net.neuron[i].x,net.neuron[i].y),net.rad,
                                                     QPointF(net.neuron[i].x,net.neuron[i].y)+0.5*QPointF(net.rad,net.rad));


            if(net.neuron[i].is_excitatory)
            {
                net.neuron[i].QCLR=QColor(0,net.neuron[i].vis,0);
                gradient.setColorAt(1.0, QColor(net.neuron[i].vis,net.neuron[i].vis,net.neuron[i].vis));
                gradient.setColorAt(0.0, net.neuron[i].QCLR );
            }
            else
            {
                net.neuron[i].QCLR=QColor(100,0.5*(200+net.neuron[i].vis),100);
                gradient.setColorAt(1.0, QColor(60,0.3*(200+net.neuron[i].vis), 60));
                gradient.setColorAt(0.0,net.neuron[i].QCLR );
            }

            painter_basic->fillPath(path,gradient);


            for(int j=0;j<net.stim_ind.size();j++)
            {
                if(net.stim_ind[j]==i)
                {
                    //     gradient(0, 0, 0, 100);
                    if(net.neuron[i].is_excitatory)
                        gradient.setColorAt(1.0,
                                            QColor(130,net.neuron[i].vis*0.5+140,255-net.neuron[i].vis));

                    //                    gradient.setColorAt(1.0, QColor(0.6*(170+net.neuron[i].vis),
                    //                                                    (net.neuron[i].vis+110)*0.4,(220-net.neuron[i].vis)));

                    else

                        gradient.setColorAt(1.0, QColor(0.5*(290+net.neuron[i].vis/1.5),0.5*(200+net.neuron[i].vis),
                                                        0.5*(200+net.neuron[i].vis)));

                    net.neuron[i].QCLR=QColor(0.5*net.neuron[i].vis,net.neuron[i].vis,.5*net.neuron[i].vis);
                    gradient.setColorAt(0.0,net.neuron[i].QCLR );
                    painter_basic->fillPath(path,gradient);

                }
            }
        }

        //PICTURE OF ACTIVITY
        float maxAct=0;
        int ind1=0;
        for(int i=0;i<net.detectors_size;i++)
        {
            bool stim=0;
            for(int j=0;j<net.stim_ind.size();j++)
            {
                if(i==net.stim_ind[j])
                {stim=1;}
            }

            if(!stim)
            if(maxAct<net.neuron[i].activity)
            {
                maxAct=net.neuron[i].activity;
                ind1=i;
            }

        }
        pen.setColor(Qt::red);
        pen.setWidth(4);
        painter_basic->setPen(pen);
        painter_basic->drawEllipse(net.neuron[ind1].x-10,net.neuron[ind1].y-10,20,20);


        pen.setColor(Qt::black);
        pen.setWidth(1);
        painter_basic->setPen(pen);
        for(int i=0;i<net.detectors_size;i++)
        {
            QPainterPath path;
            int width1=15*15/sqrt(net.size);
            QRect rect=QRect(net.neuron[i].i0*width1,net.neuron[i].j0*width1,width1,width1);
            //        painter->setPen(pen);
            path.addRect(rect);
            painter_basic->drawPath(path);
            int h=net.neuron[i].activity/(0.000001+maxAct)*255.;
            //            if(h>255)qDebug()<<h;
            QCLR.setAlpha(255);
            bool stim=0;
            for(int j=0;j<net.stim_ind.size();j++)
            {
                if(i==net.stim_ind[j])
                    stim=1;


            }

            if(stim)
            {
                QCLR.setGreen(0);
                QCLR.setRed(0);
                QCLR.setBlue(255);
            }
            else
            {
                QCLR.setGreen(h);
                QCLR.setRed(h);
                QCLR.setBlue(h);

            }
            painter_basic->fillPath(path,QCLR);
        }
    }



    redraw=0;
    delete painter;
    delete painter_basic;
}

Dialog::~Dialog()
{

}

void Dialog::changeDrawing()
{
    draw_links=!draw_links;

    if(drawing_on)
        button_drawing->setText(QString("drawing on"));
    else
        button_drawing->setText(QString("drawing off"));
}

void Dialog::weightRadChanged()
{
    net.inh_k=slider_inh_k->value()/5.*3.;
    net.weightsWithRad(slider_weight_rad->value(),slider_stim_weight->value()/10.,slider_w_probab->value());
    killDelay();
    redraw=1;
}


void drawLinkWithSpike(int i, int j, QColor& QCLR, QColor& _QCLR, QPen& pen,QPainter* painter)
{

    for(int k=0;k<4;k++)
    {

        qp.setPoint(k,net.neuron[i].arrow[j].rx[k],net.neuron[i].arrow[j].ry[k]);
    }


    QLinearGradient gradient1(net.neuron[i].x,net.neuron[i].y,
                              net.neuron[j].x,net.neuron[j].y);

    float l;

    if(net.neuron[i].syn_cnt[j].size())
    {
        gradient1.setColorAt(0, net.neuron[i].QCLR);
        for(int k=0;k<net.neuron[i].syn_cnt[j].size();k++)
        {

            l=net.neuron[i].syn_cnt[j][k]/(float)net.neuron[i].eff_dist[j];
            gradient1.setColorAt((0.+l)/1.2, QCLR);
            gradient1.setColorAt((0.1+l)/1.2, _QCLR);
            gradient1.setColorAt((0.2+l)/1.2, QCLR);
        }
        gradient1.setColorAt(1, QCLR);
        painter->setBrush(gradient1);
    }
    else
    {
        painter->setBrush(QBrush(QCLR));
    }


    pen.setColor(QColor(0,0,0,0));
    painter->setPen(pen);

    painter->drawPolygon(qp);

}


void Dialog::drawingInit(QwtPlot* d_plot, QString title)
{
    d_plot->setAutoReplot();
    d_plot->setMinimumSize(400,300);
    //_______232

    // настройка функций
    QwtPlotPicker *d_picker =
            new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft, // ассоциация с осями
                QwtPlotPicker::CrossRubberBand, // стиль перпендикулярных линий
                QwtPicker::ActiveOnly, // включение/выключение
                d_plot->canvas() ); // ассоциация с полем
    // Цвет перпендикулярных линий
    d_picker->setRubberBandPen( QColor( Qt::red ) );

    // цвет координат положения указателя
    d_picker->setTrackerPen( QColor( Qt::black ) );

    // непосредственное включение вышеописанных функций
    d_picker->setStateMachine( new QwtPickerDragPointMachine() );

    // Включить возможность приближения/удаления графика
    // #include <qwt_plot_magnifier.h>
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier(d_plot->canvas());
    // клавиша, активирующая приближение/удаление
    magnifier->setMouseButton(Qt::MidButton);
    // Включить возможность перемещения по графику
    // #include <qwt_plot_panner.h>
    QwtPlotPanner *d_panner = new QwtPlotPanner( d_plot->canvas() );
    // клавиша, активирующая перемещение
    d_panner->setMouseButton( Qt::RightButton );
    // Включить отображение координат курсора и двух перпендикулярных
    // линий в месте его отображения

    QwtText* qwtt=new QwtText(title);
    qwtt->setFont(QFont("Helvetica", 11,QFont::Normal));

    //    d_plot->setAxisScale(1,-500,500,200);
    d_plot->setTitle( *qwtt ); // заголовок
    d_plot->setCanvasBackground( Qt::white ); // цвет фона
    //    d_plot->set
}
