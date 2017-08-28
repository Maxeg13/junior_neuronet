#include "neuronizh.h"
#include "net.h"
#include <math.h>

neuronIzh::neuronIzh()
{

}

neuronIzh::neuronIzh(int _ID, neuronType _type, bool _is_exitory,CNet* _net)
{
    net=_net;
    step=0.5;
    psc_excxpire_time=4;
    is_exitory=_is_exitory;
    ID=_ID;
    type=_type;
    switch(_type)
    {
    case RS:
        a=0.02;
        b=0.2;
        c=-65;
        d=8;
        break;
    case TC:
        a=0.02;
        b=0.25;
        c=-65;
        d=0.05;
        break;
    }
    output.size();
}

void neuronIzh::CalculateStep()
{
    for(int i=0;i<net->size;i++)
        input_sum+=net->neuron[i].output[ID].back();

    input_sum*=exp(-step/psc_excxpire_time);
    
    float dE_m = 0.04*E_m*E_m + 5*E_m + 140 - U_e + input_sum;
    float dU_e = a*(b*E_m - U_e);
    E_m += net->step * dE_m;
    U_e += net->step * dU_e;

    if(E_m >= 30) // spike here! value 30 mV - by Izhikevich
    {
//        is_spike = true;
        E_m = c;
//        E_m_old = 30; // just to show the peak
        U_e = U_e + d;
    }

    for(int i=0;i<output.size();i++)
    {
        output[i].push_front(E_m);
        output[i].pop_back();
    }
}
