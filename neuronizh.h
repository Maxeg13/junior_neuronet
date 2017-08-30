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
    float x;
    float y;
    float vis;

    neuronIzh();
    neuronIzh(int, neuronType, bool, CNet*);
    neuronIzh* neighbour_neuron;
//    net* netIzh;
    int ID;
    neuronType type;
    bool is_exitory;
    deque<deque<float>> output;
    float input_sum;
    float external_I;
    float* link_length;
    float minWeight; // веса, размерность пкА
    float maxWeight;
    float psc_excxpire_time;
    float step;
    float Cm;
    float* weight_norm;
    float* weight;
    float E_m;
    float U_e;
    float a;
    float b;
    float c;
    float d;
    void test(float);
    void CalculateStep();
};

#endif // NEURONIZH_H
