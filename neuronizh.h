#ifndef NEURONIZH_H
#define NEURONIZH_H
#include <stdio.h>
#include <deque>
#include <QColor>
//#include "net.h"
using namespace std;
enum neuronType {RS, TC, IB};
class CNet;
//class neuronIzh;

class CArrow
{
public:
    float x[2],y[2], rx[4], ry[4];
    CArrow()
    {
        x[0]=0;
        x[1]=0;
        y[0]=0;
        y[1]=0;

        rx[0]=0;
        rx[1]=0;
        ry[0]=0;
        ry[1]=0;
        rx[2]=0;
        rx[3]=0;
        ry[2]=0;
        ry[3]=0;

    }
};

class neuronIzh
{
public:
    float activity;

    deque<int> top;
    CNet* net;
    CArrow* arrow;
    deque<int> rastr;
    deque<deque<int>> syn_cnt;
    QColor QCLR;
    int i;
    int i0, j0;
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
    int* to_input;
    int STDP;
    bool* STDP_set;
    int phase_noise;
    bool with_poisson;
    int pois_cnt;
    int pois_T;
    bool pois_modulator;
    int freq_phase;
    neuronType type;
    bool is_excitatory;
    int* eff_dist;
    float input_from_neurons;
    float input_sum;
    float external_I;
    float* link_length;
    int freq;
    int stim_rnd;
    unsigned int freq_cnt;
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
    int getPoisson();
    void rangedRemoted();
    void topOfRemoted(int n);
    bool isWithin2(float xx, int i);
    void oneStep();
    void setWeight(int i, float x);
    void weightsWithRad(float);
    void pull(float x1, float y1);
    void push(float x1, float y1);
    void locate();
    void locate(int _x, int _y);
    void setRandomWeight(int , bool);
};

#endif // NEURONIZH_H
