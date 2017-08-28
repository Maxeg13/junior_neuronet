#ifndef NEURONIZH_H
#define NEURONIZH_H
#include <stdio.h>
#include <deque>
//#include "net.h"
using namespace std;
enum neuronType {RS, TC};
class CNet;
//class neuronIzh;

class neuronIzh
{
public:
    CNet* net;
    float psc_excxpire_time;
    float step;
    neuronIzh();
    neuronIzh(int, neuronType, bool);
    neuronIzh* neighbour_neuron;
//    net* netIzh;
    int ID;
    neuronType type;
    bool is_exitory;
    deque<deque<float>> output;
    float input_sum;
    float* weight;
    float E_m;
    float U_e;
    float a;
    float b;
    float c;
    float d;
    void CalculateStep();
};

#endif // NEURONIZH_H
