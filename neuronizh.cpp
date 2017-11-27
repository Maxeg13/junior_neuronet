#include "neuronizh.h"
#include "cnet.h"
#include <math.h>
#include <stdlib.h>
#include <QDebug>
#include <iostream>
float max(float x,float y)
{
    if(x>y)
        return x;
    return y;
}

int neuronIzh::getPoisson()
{
//    static float T_eff=net->poisson_interval*1.9/log(2.718);
    //    static float cnt=0;
    //    static float ac=0;
    int ans=net->T_eff*(-log((rand()%40)/41.+0.001));
    //    cnt++;
    //    ac+=1./ans;
    //    qDebug()<<ac/cnt;
    return(ans);
}

neuronIzh::neuronIzh()
{

}

neuronIzh::neuronIzh(int _ID, neuronType _type, bool _is_excitatory,CNet* _net)
{


    pois_T=rand()%300;
    pois_cnt=0;
    with_poisson=1;
    STDP=2;

    vx=0;
    vy=0;

    stim_rnd=0;
    freq_cnt=0;
    vis=0;
    net=_net;
    freq_phase=1;
    step=net->step;
    is_excitatory=_is_excitatory;

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
    case IB:
        a=0.02;
        b=0.2;
        c=-55;
        d=4;
        break;
    }


    //    net->minWeight=20;
    //    net->maxWeight=70;

    E_m=c;
    U_e=d;
    input_from_neurons=0;
    freq=2;
    time_from_freq=1000/freq;
    freq_cnt=0;
    external_I=0;
    output.resize(net->size);
    STDP_set=new bool[net->size];
    weight=new float[net->size];
    weight_norm=new float[net->size];
    r1=new float[net->size];
    r2=new float[net->size];
    o1=new float[net->size];
    o2=new float[net->size];

    //syn_cnt.resize(
    //    syn_cnt=new int[net->size];
    syn_cnt.resize(net->size);
    //    for(int i=0;i<net->size;i++)
    //    syn_cnt[i].;
    for(int i=0;i<net->size;i++)
        STDP_set[i]=1;

    if(is_excitatory)
        for(int i=0;i<net->size;i++)
            if(net->neuron[i].is_excitatory)
            {
                r1[i]=0;
                r2[i]=0;
                o1[i]=0;
                o2[i]=0;
                //                syn_cnt[i]=0;
            }
    //    qDebug()<<net->width;
    locate();

    //    for(int i=0;i<net->size;i++)
    //    {
    //        if(i!=ID)weight[i]=(weight_norm[i]=(rand() % ((int)(net->maxWeight - net->minWeight)*10))/10.0f) + net->minWeight;
    //        weight_norm[i]/=(net->maxWeight - net->minWeight);
    //    }
    //    weight[ID]=0;
    //    weight_norm[ID]=0;

    arrow= new CArrow[net->size]();
}

void neuronIzh::locate()
{
    if(net->demo)
    {
        x=(net->width)*(net->size_k)+(rand()%(net->width))*(1-2*(net->size_k));
        y=(net->height)*(net->size_k)+(rand()%(net->height))*(1-2*(net->size_k));
    }
    else
    {
        if(ID<net->detectors_size)
        {
            //             x=net->width/2*1.3+(((rand()%100-50)/80.*1.5))*net->width*0.10; y=net->height*(.53+(rand()%100-50)/600.*1.5);
            static int width=sqrt(net->detectors_size);
            float x1=(ID%width+(rand()%8)*.1)*40.;
            float y1=(ID/width+(rand()%8)*.1)*40.;

            x=net->width/2*1.3+x1; y=net->height*.53+y1;
        }
        else
        {
            y=net->height*(1-(ID-net->detectors_size-3.5)*(ID-net->detectors_size-3.5)/80.);
            x=net->width/2*1.3+((ID-net->detectors_size-3.5))*net->width*0.10;
        }
        //        switch(ID)
        //        {
        //        case 0: x=net->width/2*1.3+(rand()%100)*+((ID-.5))*net->width*0.10; y=net->height*.53; break;
        //        case 1: x=net->width/2*1.3+((ID-.5))*net->width*0.10; y=net->height*.53;break;
        //        case 2:
        //        case 3:
        //        case 4:
        //        case 5:
        //        case 6:
        //        case 7:
        //        case 8:
        //        case 9:

        //            y=net->height*(1-(ID-5.5)*(ID-5.5)/80.);
        //            x=net->width/2*1.3+((ID-5.5))*net->width*0.10;
        //            break;

        //        }
    }


}

void neuronIzh::locate(int _x, int _y)
{
    x=_x;
    y=_y;
}


void neuronIzh::pull(float x1,float y1)
{
    //    qDebug()<<net->width;
    if(!((abs(x1-x)<20)&&(abs(y1-y)<20)))
    {
        float rad2=sqrt((x1-x)*(x1-x)+(y1-y)*(y1-y));
        vx=(x1-x)/rad2;
        vy=(y1-y)/rad2;
        if(x+vx>net->width*net->size_k)
            x+=vx;
        if(y+vy>net->height*net->size_k)
            y+=vy;
    }
}

void neuronIzh::push(float x1,float y1)
{


    float rad2=((x1-x)*(x1-x)+(y1-y)*(y1-y));
    vx=100*(x1-x)/rad2;
    vy=100*(y1-y)/rad2;
    if(x-vx>net->width*net->size_k)
        x-=vx;
    if(y-vy>net->height*net->size_k)
        y-=vy;

}

bool neuronIzh::isWithin2(float xx, int i)
{return xx>((x-net->neuron[i].x)*(x-net->neuron[i].x)+(y-net->neuron[i].y)*(y-net->neuron[i].y));}

void neuronIzh::rangedRemoted()
{
    deque<int> IDS;
    deque<int> ds;


    for(int i=0;i<net->detectors_size;i++)
    {
        IDS.push_back(i);

        float d=((x-net->neuron[i].x)*(x-net->neuron[i].x)+(y-net->neuron[i].y)*(y-net->neuron[i].y));
        ds.push_back(d);
    }

    int h;
    for(int j=0;j<net->detectors_size;j++)
        for(int i=1;i<net->detectors_size-j;i++)
        {
            if(ds[i-1]>ds[i])
            {
                h=ds[i];
                ds[i]=ds[i-1];
                ds[i-1]=h;

                h=IDS[i];
                IDS[i]=IDS[i-1];
                IDS[i-1]=h;
            }
        }

//    net->weight_ind.resize(net->size);
//    for(int i=0;i<net->size;i++)
        net->weight_ind=IDS;

}

void neuronIzh::topOfRemoted(int n)
{
    deque<int> IDS;
    deque<int> ds;
    top.resize(n);

    for(int i=0;i<net->detectors_size;i++)
    {
        IDS.push_back(i);

        float d=((x-net->neuron[i].x)*(x-net->neuron[i].x)+(y-net->neuron[i].y)*(y-net->neuron[i].y));
        ds.push_back(d);
    }

    int h;
    for(int j=0;j<n;j++)
        for(int i=1;i<net->detectors_size-j;i++)
        {
            if(ds[i-1]>ds[i])
            {
                h=ds[i];
                ds[i]=ds[i-1];
                ds[i-1]=h;

                h=IDS[i];
                IDS[i]=IDS[i-1];
                IDS[i-1]=h;
            }
        }

    for(int i=0;i<n;i++)
        top[i]=IDS[net->detectors_size-i-1];

}

void neuronIzh::weights_with_rad(float x1)
{
    float xx=x1*x1;
    for(int i=0;i<net->size;i++)
    {
        if(isWithin2(xx,i))
        {
            if(i!=ID)
            {
                //                weight[i]=(is_excitatory?1:(-1))*(weight_norm[i]=(rand() % ((int)(net->maxWeight - net->minWeight)*10))/10.0f) + net->minWeight;
                setRandomWeight(i,is_excitatory);
                //                        (rand() % ((int)(net->maxWeight - net->minWeight)*100))/400.0f;

                weight_norm[i]/=(net->maxWeight - net->minWeight);
                weight_norm[i]+=net->min_weight_norm;
            }
        }
        else
        {
            weight[i]=0;
            weight_norm[i]=0;
        }
    }
}

void neuronIzh::setRandomWeight(int i, bool b)
{
    weight[i]=((b?1:(-7))*(net->maxWeight + net->minWeight)/2+
               (rand()%100 -50)/100.*(net->maxWeight - net->minWeight)*net->weight_diap);

}

void neuronIzh::setWeight(int i, float x)
{
    weight[i]=x;
}

void neuronIzh::oneStep(float x)
{
    vis*=exp(-net->ext_show*x*40);//
    CalculateStep();
}

void neuronIzh::CalculateStep()
{
    if(with_poisson && net->poisson_on)
        pois_cnt++;

    if(pois_cnt>pois_T)
    {
        pois_modulator=1;
        pois_cnt=0;
        pois_T=getPoisson();
    }
    else
        pois_modulator=0;
    //    if(ID==1)qDebug()<<pois_T;

    freq_cnt++;
    if(freq_cnt+stim_rnd>(time_from_freq+1))
    {
        freq_cnt=0;
        //        freq_modulator=1;
        //        stim_rnd=rand()%4;
        stim_rnd=0;
        //        phase_noise=rand()%7;//nice
        phase_noise=rand();
    }

    if(((freq_cnt))==(freq_phase+phase_noise)%(time_from_freq+1))freq_modulator=1;
    else freq_modulator=0;


    if(ID==0) net->STDP_cnt++;//from old mistake
    if(net->STDP_cnt==net->STDP_div)
    {
        net->STDP_cnt=0;
        //qDebug()<<net->STDP_div;
    }
    if(STDP==2)
    {
        if(net->STDP_cnt==0)
        {
            if(is_excitatory)
                for(i=0;i<net->size;i++)
                    if(net->neuron[i].is_excitatory)
                    {
                        r1[i]*=net->exp_tau_p;
                        r2[i]*=net->exp_tau_x;
                        o1[i]*=net->exp_tau_m;
                        o2[i]*=net->exp_tau_y;
                    }
        }
    }


    for(i=0;i<net->size;i++)
        if(fabs(net->neuron[i].weight[ID])>net->minWeight)
            input_from_neurons+=net->neuron[i].output[ID].back()*net->neuron[i].weight[ID];
    input_sum=input_from_neurons+max(external_I*(freq_modulator),50*pois_modulator);
    //    if(external_I*freq_modulator>0.1)std::cout<<ID<<"\n";
    

    dE_m = 0.04*E_m*E_m + 5*E_m + 140 - U_e + (input_sum);
    E_m +=  dE_m*net->steph;
    dU_e = a*(b*E_m - U_e);
    U_e +=   dU_e*net->steph;

    dE_m = 0.04*E_m*E_m + 5*E_m + 140 - U_e + (input_sum);
    E_m +=  dE_m*net->steph;
    dU_e = a*(b*E_m - U_e);
    U_e +=   dU_e*net->steph;

    input_from_neurons*=net->exp_psc_exc;

    for(i=0;i<net->size;i++)
    {
        for(int j=0;j<syn_cnt[i].size();j++)
        {
            if(syn_cnt[i][j])
                syn_cnt[i][j]++;
            if(syn_cnt[i][j]>(output[i].size()))
                syn_cnt[i].pop_back();
        }
    }


    to_output=0;
    if(E_m >= 30) // spike here! value 30 mV - by Izhikevich
    {
        rastr.push_back(net->rastr_time);
        //        if(ID==15)
        //            qDebug()<<"hello!";

        for(i=0;i<net->size;i++)
            syn_cnt[i].push_front(1);

        float dw;
        to_output=1;
        vis=220;
        //        is_spike = true;
        E_m = c;
        //        E_m_old = 30; // just to show the peak
        U_e = U_e + d;

        if(STDP==2)//triplet STDP

            for(i=0;i<net->size;i++)
            {
                if(net->neuron[i].STDP_set[ID])
                    if((net->neuron[i].weight[ID]>0.0001)&&(net->neuron[i].is_excitatory))//inputs
                    {
                        //post
                        dw=(net->neuron[i].r1[ID]*
                            (net->Ap2+net->Ap3*net->neuron[i].o2[ID]))*net->STDP_speed;//(net->maxWeight-net->neuron[i].weight[ID]);

                        net->neuron[i].weight[ID]+= dw;

                        if(net->neuron[i].weight[ID]   >   net->maxWeight)
                            net->neuron[i].weight[ID]=net->maxWeight;
                        net->neuron[i].o1[ID]+=1;
                        net->neuron[i].o2[ID]+=1;

                        //                        if((i==2)&&(ID==0))
                        //                        {
                        //                            std::cout<<net->neuron[1].freq<<"  "<<dw<<"\n";
                        //                        }

                    }
                    else if((weight[i]>0.0001)&&is_excitatory)//outputs
                        if(STDP_set[i])
                        {
                            //pre
                            dw=-o1[i]*(net->Am2+net->Am3*r2[i])*net->STDP_speed;

                            weight[i]+=dw*net->alpha;//                        (weight[i]-net->minWeight);

                            if(weight[i]  <  net->minWeight)
                                weight[i]=net->minWeight;
                            r1[i]+=1;
                            r2[i]+=1;


                            //                        if((i==0)&&(ID==2))
                            //                        {
                            //                            std::cout<<net->neuron[1].freq<<"  "<<dw<<"\n";
                            //                        }
                        }


            }

    }


}
