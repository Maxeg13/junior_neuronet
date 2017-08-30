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
    int circle_val;
    neuronIzh *neuron;
    float step;
    float ext_show;
    void setLinks();
    void test(float x);
    void setArrows();
    void setDelay(int i, int j);
    void spikesStop();
};

#endif // NET_H
