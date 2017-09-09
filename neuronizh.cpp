#include "neuronizh.h"
#include "cnet.h"
#include <math.h>
#include <stdlib.h>
#include <QDebug>
#include <iostream>

neuronIzh::neuronIzh()
{

}

neuronIzh::neuronIzh(int _ID, neuronType _type, bool _is_excitatory,CNet* _net)
{
    vx=0;
    vy=0;


    //    float k=0.15;

    //    int wh=6;
    //    int hh=6;
    //    float h1=(rand())%width-width/2;
    //    float h2=(rand())%height-height/2;
    //    x=h1*h1*((h1>0)?1:(-1))/wh/wh*.6+width;
    //    y=h2*h2*((h2>0)?1:(-1))/hh/hh*.6+height;

    //    float loc_rad=width/3;

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

    freq_cnt=0;
    vis=0;
    net=_net;
    step=net->step;
    is_excitatory=_is_excitatory;

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

    E_m=c;
    U_e=d;
    input_sum=0;
    external_I=0;
    output.resize(net->size);
    weight=new float[net->size];
    weight_norm=new float[net->size];
    r1=new float[net->size];
    r2=new float[net->size];
    o1=new float[net->size];
    o2=new float[net->size];

    if(is_excitatory)
        for(int i=0;i<net->size;i++)
            if(net->neuron[i].is_excitatory)
            {
                r1[i]=0;
                r2[i]=0;
                o1[i]=0;
                o2[i]=0;
            }
    //    qDebug()<<net->width;
    locate();
    weights_with_rad(600);
    //    for(int i=0;i<net->size;i++)
    //    {
    //        if(i!=ID)weight[i]=(weight_norm[i]=(rand() % ((int)(net->maxWeight - net->minWeight)*10))/10.0f) + net->minWeight;
    //        weight_norm[i]/=(net->maxWeight - net->minWeight);
    //    }
    //    weight[ID]=0;
    //    weight_norm[ID]=0;

    arrow= new CArrow[net->size]();
}

void neuronIzh::locate()
{

    x=(net->width)*(net->size_k)+(rand()%(net->width))*(1-2*(net->size_k));

    y=(net->height)*(net->size_k)+(rand()%(net->height))*(1-2*(net->size_k));
    //    x=rand()%400;
    //    y=rand()%400;
}

void neuronIzh::pull(float x1,float y1)
{
    //    qDebug()<<net->width;
    if(!((abs(x1-x)<20)&&(abs(y1-y)<20)))
    {
        float rad2=sqrt((x1-x)*(x1-x)+(y1-y)*(y1-y));
        vx=(x1-x)/rad2;
        vy=(y1-y)/rad2;
        if(x+vx>net->width*net->size_k)
            x+=vx;
        if(y+vy>net->height*net->size_k)
            y+=vy;
    }
}

void neuronIzh::push(float x1,float y1)
{


    float rad2=((x1-x)*(x1-x)+(y1-y)*(y1-y));
    vx=100*(x1-x)/rad2;
    vy=100*(y1-y)/rad2;
    if(x-vx>net->width*net->size_k)
        x-=vx;
    if(y-vy>net->height*net->size_k)
        y-=vy;

}

void neuronIzh::weights_with_rad(float x1)
{
    float xx=x1*x1;
    for(int i=0;i<net->size;i++)
    {
        if(xx>((x-net->neuron[i].x)*(x-net->neuron[i].x)+(y-net->neuron[i].y)*(y-net->neuron[i].y)))
        {
            if(i!=ID)
            {
                weight[i]=(is_excitatory?1:(-1))*(weight_norm[i]=(rand() % ((int)(net->maxWeight - net->minWeight)*10))/10.0f) + net->minWeight;
                weight_norm[i]+=0.1;
                weight_norm[i]/=(net->maxWeight - net->minWeight);
            }
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
    freq_cnt++;
    if(freq_cnt==time_from_freq)
    {
        freq_cnt=0;
        freq_modulator=1;
    }
    else if(freq_cnt==(10))freq_modulator=0;


    net->STDP_cnt++;
    if(net->STDP_cnt==net->STDP_div)
    {
        net->STDP_cnt=0;
        //qDebug()<<net->STDP_div;
    }
    if(net->STDP==2)
    {
        if(net->STDP_cnt==0)
        {
            if(is_excitatory)
                for(i=0;i<net->size;i++)
                    if(net->neuron[i].is_excitatory)
                    {
                        r1[i]*=net->exp_tau_p;
                        r2[i]*=net->exp_tau_x;
                        o1[i]*=net->exp_tau_m;
                        o2[i]*=net->exp_tau_y;
                    }
        }
    }


    for(i=0;i<net->size;i++)
        if(net->neuron[i].weight[ID]!=0)
            input_sum+=net->neuron[i].output[ID].back()*net->neuron[i].weight[ID];


    

    dE_m = 0.04*E_m*E_m + 5*E_m + 140 - U_e + (input_sum+external_I*freq_modulator);
    E_m +=  dE_m*net->steph;
    dE_m = 0.04*E_m*E_m + 5*E_m + 140 - U_e + (input_sum+external_I*freq_modulator);
    E_m +=  dE_m*net->steph;


    dU_e = a*(b*E_m - U_e);
    U_e +=   dU_e*net->step;

    input_sum*=net->exp_psc_exc;


    to_output=0;
    if(E_m >= 30) // spike here! value 30 mV - by Izhikevich
    {
        float dw;
        to_output=1;
        vis=220;
        //        is_spike = true;
        E_m = c;
        //        E_m_old = 30; // just to show the peak
        U_e = U_e + d;

        if(net->STDP==2)//triplet STDP
            if(is_excitatory)
            {

                for(i=0;i<net->size;i++)
                {

                    if(net->neuron[i].weight[ID]&&(net->neuron[i].is_excitatory))//inputs
                    {
                        //post
                        dw=(net->neuron[i].r1[ID]*
                            (net->Ap2+net->Ap3*net->neuron[i].o2[ID]))/50;//(net->maxWeight-net->neuron[i].weight[ID]);
                        net->neuron[i].weight[ID]+= dw;

                        if(net->neuron[i].weight[ID]   >   net->maxWeight)
                            net->neuron[i].weight[ID]=net->maxWeight;
                        net->neuron[i].o1[ID]+=1;
                        net->neuron[i].o2[ID]+=1;

                        //                        if((i==2)&&(ID==0))
                        //                        {
                        //                            std::cout<<net->neuron[1].freq<<"  "<<dw<<"\n";
                        //                        }

                    }
                    else if(weight[i]&&net->neuron[i].is_excitatory)//outputs
                    {
                        //pre
                        dw=-o1[i]*(net->Am2+net->Am3*r2[i])/50;
                        weight[i]+=dw;//                        (weight[i]-net->minWeight);

                        if(weight[i]  <  net->minWeight)
                            weight[i]=net->minWeight;
                        r1[i]+=1;
                        r2[i]+=1;


                        //                        if((i==0)&&(ID==2))
                        //                        {
                        //                           std::cout<<net->neuron[1].freq<<"  "<<dw<<"\n";
                        //                        }
                    }

                }
            }

    }


}
