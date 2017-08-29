#include "cnet.h"
#include <stdlib.h>
void CNet::test()
{
    for(int i=0;i<size;i++)
        neuron[i].test();
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
}

void CNet::setLinks()
{
    //    for(int i=0;i<size;i++)
    //        for(int )
}
