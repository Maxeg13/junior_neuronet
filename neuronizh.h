#ifndef NEURONIZH_H
#define NEURONIZH_H
#include <stdio.h>
#include <deque>
//#include "net.h"
using namespace std;
enum neuronType {RS, TC};
class CNet;
//class neuronIzh;

class CArrow
{
public:
    float x[2],y[2];
    CArrow()
    {
        x[0]=0;
        x[1]=0;
        y[0]=0;
        y[1]=0;
    }
};

class neuronIzh
{
public:
    CNet* net;
    CArrow* arrow;
    int i;
    float x;
    float y;
    float vx;
    float vy;
    float vis;    
    neuronIzh();
    neuronIzh(int, neuronType, bool, CNet*);
    neuronIzh* neighbour_neuron;
//    net* netIzh;
    int ID;
    float to_output;
    neuronType type;
    bool is_excitatory;
    deque<deque<float>> output;
    float input_sum;
    float input_from_neurons;
    float external_I;
    float* link_length;
    int freq;
    int freq_cnt;
    int time_from_freq;
    int freq_modulator;
//    float psc_excxpire_time;
    float step;
    float* weight_norm;
    float* weight;
    float* r1;
    float* r2;
    float* o1;
    float* o2;
    float E_m;
    float U_e;
    float dE_m;
    float dU_e;
    float a;
    float b;
    float c;
    float d;
    void test(float);
    void CalculateStep();
    void weights_with_rad(float);
    void pull(float x1, float y1);
    void push(float x1, float y1);
    void locate();
};

#endif // NEURONIZH_H
