//
// Created by Admin on 13.06.2019.
//

#include "ADSR.h"

ADSR::ADSR()
{}

void ADSR::Attack(float A_, float D_, float S_, float R_)
{
    released = false;
    A = sample_rate*A_;
    D = sample_rate*D_;
    R = sample_rate*R_;
    S = S_;
    if (amp < 0) amp = 0;
    position = (int)(amp*A);
    ad = 1.0f/A;
    ds = (S - 1.0F)/D;
    sr = -S/R;
}

void ADSR::Release(std::function<void(bool)> callback_)
{
    callback = callback_;
    released = true;
}

void ADSR::ARender(double beat, float *lsample, float *rsample) {
    if (released){
        if (amp > 0) {
            amp += sr;
        } else {
            amp = 0;
            released = false;
            callback(false);
        }
    } else if (position < A){
        amp += ad;
    } else if (position < A + D){
        amp += ds;
    }

    position ++;
    if (amp > 1) amp = 1;
}