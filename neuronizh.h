#ifndef NEURONIZH_H
#define NEURONIZH_H
#include <stdio.h>
#include <deque>
using namespace std;
enum neuronType {RS, TC};

class neuronIzh
{
public:
    neuronIzh();
    neuronIzh(int, neuronType, bool);
    int ID;
    neuronType type;
    bool is_exitory;
    deque<deque<float>> input;
    float input_sum;
    float* weight;
    float a;
    float b;
    float c;
    float d;
    void CalculateStep();
};

#endif // NEURONIZH_H
