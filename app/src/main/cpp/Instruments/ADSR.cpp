//
// Created by Admin on 13.06.2019.
//

#include "ADSR.h"

ADSR::ADSR()
{
    A = 5000;
    D = 5000;
    S = 0.7;
    R = 10000;
    amp = 0.0001;
}

void ADSR::attack()
{
    active = true;
    if (amp < 0) amp = 0.00001;
    position = (int)(amp/A);
    amp_inc = 1.0F/(float)A;
}

void ADSR::release()
{
    amp_inc = -S/(float)R;
}

float ADSR::apply(float sample)
{
    if (position == A){
        amp_inc = (S - 1.0F)/(float)(D);
    }
    if (position == A + D){
        release();
    }
    if(amp > 0){
        amp += amp_inc;
        position ++;
    } else {
        active = false;
    }
    return sample*amp;
}