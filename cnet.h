#ifndef NET_H
#define NET_H
#include "neuronizh.h"
#include <vector>
//#define neuronsType neuronType;
//enum neuronType;

class CNet
{
public:
    float alpha;
    deque<int> weight_ind;
    bool poisson_on;
    int poisson_interval;
    float T_eff;
    int rastr_time;
    float min_weight_norm;
    float weight_diap;
    float psc_excxpire_time;
    float exp_activity;
    float exp_psc_exc;
    CNet();
    CNet(int _size, int _perc, neuronType _type);
    void kohonSettings();
    neuronType type;
    bool demo;
    int a;
    int size;
    int detectors_size;
    float size_k;
    int width;
    int height;
    int rad;
    float STDP_speed;
    int weight_rad;
    int inhibitory_perc;
    neuronIzh *neuron;
    float Ap2;
    float Ap3;
    float Am2;
    float Am3;
    float exp_tau_p;
    float exp_tau_x;
    float exp_tau_m;
    float exp_tau_y;
    float tau_p;
    float tau_x;
    float tau_m;
    float tau_y;
    float step;
    float steph;
    float ext_show;
    bool spike_show;

    int STDP_cnt;
    int STDP_div;
    void CalculateStep(float x);
    void setArrows();
    void setDelay(int i, int j);
    void spikesStop();
    std::vector<int> stim_ind;
    float minWeight; // веса, размерность пкА
    float maxWeight;
    void weights_with_rad(float x1);
    void afterReWeight();
    void normWeights();
    void demoSettings(int x);

//    CNet(int _size, int _perc, neuronType /*_type);*/
};

#endif // NET_H
