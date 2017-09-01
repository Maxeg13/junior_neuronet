#ifndef NET_H
#define NET_H
#include "neuronizh.h"
#include <vector>
//#define neuronsType neuronType;
//enum neuronType;

class CNet
{
public:

    CNet();
    CNet(int , neuronType);
    neuronType type;
    int size;
    int rad;
    int weight_rad;
    int circle_val;
    neuronIzh *neuron;
    float step;
    float ext_show;
    void setLinks();
    void test(float x);
    void setArrows();
    void setDelay(int i, int j);
    void spikesStop();
    std::vector<int> stim_ind;
    void weights_with_rad(float x1);
    void afterReWeight();
};

#endif // NET_H
