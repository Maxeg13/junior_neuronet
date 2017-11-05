#include "cnet.h"
#include <stdlib.h>
#include <math.h>
#include <QDebug>

float thresh(int x)
{
    if(x>1)
        return(1);
    else
        return(x);
}

CNet::CNet(int _size,int _perc, neuronType _type):a(100)
{
    //modes:
    demo=0;
    spike_show=1;

    min_weight_norm=0.4;
    minWeight=1/20.;//for experiment
    maxWeight=8;
    step=1;
    size_k=.2;
    psc_excxpire_time=10;//4,0.1
    exp_psc_exc=exp(-step/psc_excxpire_time);
    if(demo)
        weight_diap=.99;
    else
        weight_diap=.12;

    steph=step/2;



    width=450;
    height=380;
    STDP_cnt=0;
    STDP_div=1;


    tau_p=16;
    tau_x=100;
    tau_m=30;
    tau_y=125;

//    Am2=.007;
//    Am3=.00023;
//    Ap2=5*.0000000001;
//    Ap3=.0062;
    Am2=.0071;
    Am3=0;
    Ap2=0;
    Ap3=.0065;





    exp_tau_p=exp(-step*STDP_div/tau_p);
    exp_tau_x=exp(-step*STDP_div/tau_x);
    exp_tau_m=exp(-step*STDP_div/tau_m);
    exp_tau_y=exp(-step*STDP_div/tau_y);

    //    Am2=.007;
    //    Am3=.007;
    //    Ap2=7*.001;
    //    Ap3=.007;


    //    minWeight=.1;

    //    maxWeight=7;
    detectors_size=10;
    rad=6;
    inhibitory_perc=_perc;
    //    circle_val=50;
    size=_size;
    type=_type;

    ext_show=0.001;
    neuron=new neuronIzh[size]();
    for(int i=0;i<size;i++)
        neuron[i]= neuronIzh(i,_type,((rand()%100)>(inhibitory_perc-1)),this);

//    testSettings(600);

    kohonSettings();
}

void CNet::demoSettings(int x)
{
    for(int i=0;i<size;i++)
    neuron[i].weights_with_rad(x);
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
            if(neuron[i].weight[j]>0.0001)
                neuron[i].weight_norm[j]=(neuron[i].weight[j]-minWeight)/(maxWeight-minWeight)+min_weight_norm;

            if(neuron[j].weight[i]>0.0001)
                neuron[j].weight_norm[i]=(neuron[j].weight[i]-minWeight)/(maxWeight-minWeight)+min_weight_norm;

            if(neuron[i].weight[j]<-0.0001)
                neuron[i].weight_norm[j]=thresh(-(neuron[i].weight[j]-minWeight)/(maxWeight-minWeight))+min_weight_norm;

            if(neuron[j].weight[i]<-0.0001)
                neuron[j].weight_norm[i]=thresh(-(neuron[j].weight[i]-minWeight)/(maxWeight-minWeight))+min_weight_norm;
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



void CNet::kohonSettings()
{
    for(int j=0;j<size;j++)
        for(int i=0;i<size;i++)
            neuron[i].setWeight(j,0);

    for(int j=0;j<detectors_size;j++)
        for(int i=detectors_size;i<size;i++)
            neuron[i].setWeight(j,maxWeight);

//    for(int i=2;i<size;i++)
//    {
//        neuron[i].weight[i+8]=maxWeight*2.5;
//        neuron[i].STDP_set[i+8]=0;
//        neuron[i+8].setWeight(0,maxWeight);
//        neuron[i+8].setWeight(1,maxWeight);
//    }

    for(int i=0;i<detectors_size;i++)
        for(int j=0;j<detectors_size;j++)
            if(i!=j)
    neuron[i].setWeight(j,-3*maxWeight);
//     neuron[1].setWeight(0,-4*maxWeight);

    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            setDelay(i,j);
    setArrows();
}

void CNet::setDelay(int i,int j)
{
    //////////////////////////////////////////////////
    if((neuron[i].weight[j])>0.0001)
    {
        float ex=neuron[i].x-neuron[j].x;
        float ey=neuron[i].y-neuron[j].y;
        float square=ex*ex+ey*ey+0.001;

        //const
        neuron[i].output[j].resize(1+sqrt(square)/6,0);//4
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
        neuron[i].oneStep(x);

    for(int j=0;j<size;j++)
        for(int i=0;i<neuron[j].output.size();i++)
        {
            //            if(!((j==2)&&(i==0)))//test
            {
                neuron[j].output[i].push_front(neuron[j].to_output);
                neuron[j].output[i].pop_back();
            }
        }
}
void CNet::setArrows()
{
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
        {
            if(fabs(neuron[i].weight[j])>0.0001)
            {
                float ex=neuron[i].x-neuron[j].x;
                float ey=neuron[i].y-neuron[j].y;
                float square=ex*ex+ey*ey+0.001;
                ex=ex/sqrt(square);
                ey=ey/sqrt(square);
                float phi=0.25;
                float width=2;
                int length=this->rad+4;
                neuron[i].arrow[j].x[0]=length*(ex*cos(phi)+ey*sin(phi));
                //               qDebug()<<neuron[i].arrow[j].x[0];
                neuron[i].arrow[j].y[0]=length*(-ex*sin(phi)+ey*cos(phi));
                neuron[i].arrow[j].x[1]=length*(ex*cos(phi)-ey*sin(phi));
                neuron[i].arrow[j].y[1]=length*(ex*sin(phi)+ey*cos(phi));
                if(spike_show)
                {

                    neuron[i].arrow[j].rx[0]=ey*width+neuron[i].x;
                    neuron[i].arrow[j].rx[1]=-ey*width+neuron[i].x;
                    neuron[i].arrow[j].ry[0]=-ex*width+neuron[i].y;
                    neuron[i].arrow[j].ry[1]=ex*width+neuron[i].y;

                    neuron[i].arrow[j].rx[3]=ey*width+neuron[j].x;
                    neuron[i].arrow[j].rx[2]=-ey*width+neuron[j].x;
                    neuron[i].arrow[j].ry[3]=-ex*width+neuron[j].y;
                    neuron[i].arrow[j].ry[2]=ex*width+neuron[j].y;

                }
            }
        }
}


