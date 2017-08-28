#ifndef NET_H
#define NET_H
#include "neuronizh.h"
//#define neuronsType neuronType;

class net
{
public:

    net();
    net(int , neuronType);
    neuronType type;
    int size;
    neuronIzh *neuron;

};

#endif // NET_H
