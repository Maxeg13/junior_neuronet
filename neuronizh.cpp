#include "neuronizh.h"

neuronIzh::neuronIzh()
{

}

neuronIzh::neuronIzh(int _ID, neuronType _type, bool _exitory)
{
    is_exitory=_exitory;
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
}
