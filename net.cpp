#include "net.h"
#include <stdlib.h>

CNet::CNet(int _size, neuronType _type)
{
    
size=_size;
type=_type;
step=0.5;

neuron=new neuronIzh[size]();
for(int i=0;i<size;i++)
    neuron[i]= neuronIzh(i,RS,rand()%2,this);
}
