#include "neuronizh.h"
#include "cnet.h"
#include <math.h>
#include <stdlib.h>


neuronIzh::neuronIzh()
{

}

neuronIzh::neuronIzh(int _ID, neuronType _type, bool _is_exitory,CNet* _net)
{


    int width=400;
    int height=400;
//    int wh=6;
//    int hh=6;
//    float h1=(rand())%width-width/2;
//    float h2=(rand())%height-height/2;
//    x=h1*h1*((h1>0)?1:(-1))/wh/wh*.6+width;
//    y=h2*h2*((h2>0)?1:(-1))/hh/hh*.6+height;
   x=rand()%width;
   y=rand()%height;
   float rad=80;

    while((x-width/2)*(x-width/2)+(y-height/2)*(y-height/2)>rad*rad)
    {
        x=rand()%width;
        y=rand()%height;
    }

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


    output.resize(net->size);
    weight=new float[net->size];
    for(int i=0;i<net->size;i++)
        if(i!=ID)weight[i]=rand()%10/10.;
    weight[ID]=0;

    arrow= new CArrow[net->size]();
}

void neuronIzh::test()
{
    vis*=exp(-0.001);
}

void neuronIzh::CalculateStep()
{
    for(int i=0;i<net->size;i++)
        input_sum+=net->neuron[i].output[ID].back()*net->neuron[i].weight[ID];

    input_sum*=exp(-step/psc_excxpire_time);
    
    float dE_m = 0.04*E_m*E_m + 5*E_m + 140 - U_e + input_sum;
    float dU_e = a*(b*E_m - U_e);
    E_m += net->step * dE_m;
    U_e += net->step * dU_e;

    if(E_m >= 30) // spike here! value 30 mV - by Izhikevich
    {
//        is_spike = true;
        E_m = c;
//        E_m_old = 30; // just to show the peak
        U_e = U_e + d;
    }

    for(int i=0;i<output.size();i++)
    {
        output[i].push_front(E_m);
        output[i].pop_back();
    }
}
