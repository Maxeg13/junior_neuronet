#include "cnet.h"
#include <stdlib.h>
#include <math.h>
#include <QDebug>


CNet::CNet(int _size,int _perc, neuronType _type):a(100)
{
//    a=100;
    size_k=0.1;
    width=400;
    height=350;
    STDP_cnt=0;
    STDP_div=5;


    tau_p=20;
    tau_x=20;
    tau_m=20;
    tau_y=20;

//    Am2=.007;
//    Am3=.00023;
//    Ap2=5*.0000000001;
//    Ap3=.0062;

    Am2=.007;
    Am3=.007;
    Ap2=7*.001;
    Ap3=.007;


    STDP=2;
    minWeight=4;
    maxWeight=8;
    rad=4;
    inhibitory_perc=_perc;
    //    circle_val=50;
    size=_size;
    type=_type;
    step=0.5;

    ext_show=0.001;
    neuron=new neuronIzh[size]();
    for(int i=0;i<size;i++)
        neuron[i]= neuronIzh(i,_type,((rand()%100)>(inhibitory_perc-1)),this);

    afterReWeight();


    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
        {
            setDelay(i,j);
        }
}

void CNet::normWeights()
{
    for (int i=0;i<size;i++)
    {
        for (int j=0;j<size;j++)
        {
            if(neuron[i].weight[j]!=0)
                neuron[i].weight_norm[j]=fabs(neuron[i].weight[j]-minWeight)/(maxWeight-minWeight)+.1;

            if(neuron[j].weight[i]!=0)
                neuron[j].weight_norm[i]=fabs(neuron[j].weight[i]-minWeight)/(maxWeight-minWeight)+.1;
        }
    }
}

void CNet::spikesStop()
{
    for(int i=0;i<size;i++)
    {
        neuron[i].E_m=neuron[i].c;
        neuron[i].U_e=0;
    }
}

void CNet::weights_with_rad(float x1)
{
    for(int i=0;i<size;i++)
    {
        neuron[i].weights_with_rad(x1);
    }

    afterReWeight();
}

void CNet::afterReWeight()
{
    for(int i=0;i<size;i++)
    {
        for(int j=0;j<size;j++)
        {
            if(j>i)
            {
                int case_num=rand()%2;

                switch(case_num)
                {
                case 0:
                    neuron[i].weight[j]=0;
                    neuron[i].weight_norm[j]=0;
                    break;
                case 1:
                    neuron[j].weight[i]=0;
                    neuron[j].weight_norm[i]=0;
                }
            }
        }
    }
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            setDelay(i,j);
    setArrows();
}


void CNet::setDelay(int i,int j)
{
    if(neuron[i].weight[j]!=0)
    {
        float ex=neuron[i].x-neuron[j].x;
        float ey=neuron[i].y-neuron[j].y;
        float square=ex*ex+ey*ey+0.001;

        //const
        neuron[i].output[j].resize(1+sqrt(square)/1,0);
        //        qDebug()<< neuron[i].output[j].size();
    }
    else
    {
        neuron[i].output[j].resize(1,0);
    }

}

void CNet::CalculateStep(float x)
{
    for(int i=0;i<size;i++)
        neuron[i].test(x);

    for(int j=0;j<size;j++)
        for(int i=0;i<neuron[j].output.size();i++)
        {
            neuron[j].output[i].push_front(neuron[j].to_output);
            neuron[j].output[i].pop_back();
        }
}
void CNet::setArrows()
{
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
        {
            if(neuron[i].weight[j]!=0)
            {
                float ex=neuron[i].x-neuron[j].x;
                float ey=neuron[i].y-neuron[j].y;
                float square=ex*ex+ey*ey+0.001;
                ex=ex/sqrt(square);
                ey=ey/sqrt(square);
                float phi=0.1;
                int length=this->rad+2;
                neuron[i].arrow[j].x[0]=length*(ex*cos(phi)+ey*sin(phi));
                //               qDebug()<<neuron[i].arrow[j].x[0];
                neuron[i].arrow[j].y[0]=length*(-ex*sin(phi)+ey*cos(phi));
                neuron[i].arrow[j].x[1]=length*(ex*cos(phi)-ey*sin(phi));
                neuron[i].arrow[j].y[1]=length*(ex*sin(phi)+ey*cos(phi));
            }
        }
}


