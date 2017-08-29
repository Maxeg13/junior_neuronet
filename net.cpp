#include "cnet.h"
#include <stdlib.h>
#include <math.h>
#include <QDebug>
void CNet::test()
{
    for(int i=0;i<size;i++)
        neuron[i].test();
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
                int length=8;
                neuron[i].arrow[j].x[0]=length*(ex*cos(phi)+ey*sin(phi));
//               qDebug()<<neuron[i].arrow[j].x[0];
                neuron[i].arrow[j].y[0]=length*(-ex*sin(phi)+ey*cos(phi));
                neuron[i].arrow[j].x[1]=length*(ex*cos(phi)-ey*sin(phi));
                neuron[i].arrow[j].y[1]=length*(ex*sin(phi)+ey*cos(phi));
            }
        }
}
CNet::CNet(int _size, neuronType _type)
{    
    size=_size;
    type=_type;
    step=0.5;

    neuron=new neuronIzh[size]();
    for(int i=0;i<size;i++)
        neuron[i]= neuronIzh(i,RS,rand()%2,this);

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
                    break;
                case 1:
                    neuron[j].weight[i]=0;
                }
            }
        }
    }
    setArrows();
}

void CNet::setLinks()
{
    //    for(int i=0;i<size;i++)
    //        for(int )
}
