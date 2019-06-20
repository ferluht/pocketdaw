//
// Created by Admin on 13.06.2019.
//

#include "ADSR.h"

ADSR::ADSR()
{
    A = 200;
    D = 1000;
    S = 0.7;
    R = 1000;
}

void ADSR::attack()
{
    position = 0;
    amp = 0.01;
    amp_inc = 1.0F/(float)A;
}

void ADSR::release()
{
    amp_inc = -S/(float)R;
}

float ADSR::apply(float sample)
{
    if (position == D){
        amp_inc = (S - 1.0F)/(float)(D-A);
    }
    if(amp > 0){
        amp += amp_inc;
        position ++;
    }
    return sample*amp;
}