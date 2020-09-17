//
// Created by Admin on 13.09.2019.
//

#ifndef PD_MOOGFILTER_H
#define PD_MOOGFILTER_H


#include <GUI/Encoder.h>
#include "AudioEffect.h"

class MoogFilter : public AudioEffect
{
    GUI::AnalogEncoder * cutoff_enc;
    GUI::AnalogEncoder * resonance_enc;
    GUI::AnalogEncoder * drive_enc;

public:
    MoogFilter();
    void init();
    void calc();
    float getCutoff();
    void setCutoff(float c);
    float getRes();
    void setRes(float r);
    bool ARender(double beat, float * lsample, float * rsample) override;
    void MRender(double beat) override;
protected:
    float cutoff;
    float res;
    float fs;
    float y1,y2,y3,y4;
    float oldx;
    float oldy1,oldy2,oldy3;
    float x;
    float r;
    float p;
    float k;
};


#endif //PD_MOOGFILTER_H
