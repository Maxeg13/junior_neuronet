#ifndef NET_H
#define NET_H
#include "neuronizh.h"
//#define neuronsType neuronType;
//enum neuronType;

class CNet
{
public:

    CNet();
    CNet(int , neuronType);
    neuronType type;
    int size;
    neuronIzh *neuron;
    float step;
    void setLinks();
    void test();
};

#endif // NET_H
