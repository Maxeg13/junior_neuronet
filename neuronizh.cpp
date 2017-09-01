#include "neuronizh.h"
#include "cnet.h"
#include <math.h>
#include <stdlib.h>
#include <QDebug>

neuronIzh::neuronIzh()
{

}

neuronIzh::neuronIzh(int _ID, neuronType _type, bool _is_exitory,CNet* _net)
{
    vx=0;
    vy=0;
    float k=0.15;
    int width=400;
    int height=350;
    //    int wh=6;
    //    int hh=6;
    //    float h1=(rand())%width-width/2;
    //    float h2=(rand())%height-height/2;
    //    x=h1*h1*((h1>0)?1:(-1))/wh/wh*.6+width;
    //    y=h2*h2*((h2>0)?1:(-1))/hh/hh*.6+height;
    x=width*k+(rand()%width)*(1-2*k);
    y=height*k+(rand()%height)*(1-2*k);
    float loc_rad=width/3;

    //    while((x-width/2)*(x-width/2)+(y-height/2)*(y-height/2)>loc_rad*loc_rad)
    //    {
    //        x=rand()%width;
    //        y=rand()%height;
    //    }

//    while(((x-width/4)*(x-width/2)+(y-height/3)*(y-height/3)>loc_rad*loc_rad)&&
//          ((x-width*3/4.)*(x-width*3/4.)+(y-height/3)*(y-height/3)>loc_rad*loc_rad))
//    {
//        x=rand()%width;
//        y=rand()%height;
//    }

    vis=0;
    net=_net;
    step=net->step;
    psc_excxpire_time=4;
    is_exitory=_is_exitory;

    ID=_ID;
    type=_type;
    switch(_type)
    {
    case RS:
        a=0.02;
        b=0.2;
        c=-65;
        d=8;
        break;
    case TC:
        a=0.02;
        b=0.25;
        c=-65;
        d=0.05;
        break;
    }


    //    net->minWeight=20;
    //    net->maxWeight=70;
    Cm      = 25;
    E_m=c;
    U_e=d;
    input_sum=0;
    external_I=0;
    output.resize(net->size);
    weight=new float[net->size];
    weight_norm=new float[net->size];
    for(int i=0;i<net->size;i++)
    {
        if(i!=ID)weight[i]=(weight_norm[i]=(rand() % ((int)(net->maxWeight - net->minWeight)*10))/10.0f) + net->minWeight;
        weight_norm[i]/=(net->maxWeight - net->minWeight);
    }
    weight[ID]=0;
    weight_norm[ID]=0;

    arrow= new CArrow[net->size]();
}

void neuronIzh::pull(float x1,float y1)
{

    if((abs(x1-x)>20)&&(abs(y1-y)>20))
    {
        float rad2=(x1-x)*(x1-x)+(y1-y)*(y1-y);
        vx=100*(x1-x)/rad2;
        vy=100*(y1-y)/rad2;
    x+=vx;
    y+=vy;
    }
}

void neuronIzh::weights_with_rad(float x1)
{
    float xx=x1*x1;
    for(int i=0;i<net->size;i++)
    {
        if(xx>((x-net->neuron[i].x)*(x-net->neuron[i].x)+(y-net->neuron[i].y)*(y-net->neuron[i].y)))
        {
            if(i!=ID)weight[i]=(weight_norm[i]=(rand() % ((int)(net->maxWeight - net->minWeight)*10))/10.0f) + net->minWeight;
            weight_norm[i]/=(net->maxWeight - net->minWeight);


        }
        else
        {
            weight[i]=0;
            weight_norm[i]=0;
        }
    }
}

void neuronIzh::test(float x)
{
    vis*=exp(-net->ext_show*x*40);//
    CalculateStep();
}

void neuronIzh::CalculateStep()
{
//        input_sum=0;

    for(int i=0;i<net->size;i++)
        if(net->neuron[i].weight[ID]!=0)
            input_sum+=net->neuron[i].output[ID].back()*net->neuron[i].weight[ID];

    input_sum*=exp(-step/psc_excxpire_time);
    
    float dE_m = 0.04*E_m*E_m + 5*E_m + 140 - U_e + (input_sum+external_I)/Cm;
    float dU_e = a*(b*E_m - U_e);
    E_m += net->step * dE_m;
    U_e += net->step * dU_e;

    //    if(ID==0)
    //    qDebug()<<"ID 0   "<<E_m;
    //    if(ID==1)
    //    qDebug()<<"ID 1   "<<E_m;
    float to_output=0;
    if(E_m >= 30) // spike here! value 30 mV - by Izhikevich
    {
        to_output=1;
        vis=220;
        //        is_spike = true;
        E_m = c;
        //        E_m_old = 30; // just to show the peak
        U_e = U_e + d;

    }

    for(int i=0;i<output.size();i++)
    {
        output[i].push_front(to_output);
        output[i].pop_back();
    }
}
