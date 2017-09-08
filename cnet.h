#ifndef NET_H
#define NET_H
#include "neuronizh.h"
#include <vector>
//#define neuronsType neuronType;
//enum neuronType;

class CNet
{
public:

    float psc_excxpire_time;
    CNet();
    CNet(int ,int, neuronType);
    neuronType type;
    int a;
    int size;
    float size_k;
    int width;
    int height;
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
    float steph;
    float ext_show;
    int STDP_cnt;
    int STDP_div;
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
    void normWeights();
};

#endif // NET_H
