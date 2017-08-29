#ifndef NEURONIZH_H
#define NEURONIZH_H
#include <stdio.h>
#include <deque>
//#include "net.h"
using namespace std;
enum neuronType {RS, TC};
class CNet;
//class neuronIzh;

class arrow
{
public:
    float x[2],y[2];
};

class neuronIzh
{
public:
    CNet* net;
    arrow* _arrow;
    float psc_excxpire_time;
    float step;
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
    float* link_length;
    float* weight;
    float E_m;
    float U_e;
    float a;
    float b;
    float c;
    float d;
    void test();
    void CalculateStep();
};

#endif // NEURONIZH_H
