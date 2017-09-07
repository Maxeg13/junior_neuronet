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
    CNet(int ,int, neuronType);
    neuronType type;
    int size;
    int rad;
    int weight_rad;
    int inhibitory_perc;
    neuronIzh *neuron;
    float Ap2;
    float Ap3;
    float Am2;
    float Am3;
    float tau_p;
    float tau_x;
    float tau_m;
    float tau_y;
    float step;
    float ext_show;
    void CalculateStep(float x);
    void setArrows();
    void setDelay(int i, int j);
    void spikesStop();
    std::vector<int> stim_ind;
    int STDP;
    float minWeight; // веса, размерность пкА
    float maxWeight;
    void weights_with_rad(float x1);
    void afterReWeight();
};

#endif // NET_H
