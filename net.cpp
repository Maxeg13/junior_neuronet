#include "net.h"

CNet::CNet(int _size, neuronType _type)
{
    
size=_size;
type=_type;
step=0.5;

neuron=new neuronIzh[size]();
}
