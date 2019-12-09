//
// Created by Admin on 25.07.2019.
//

#ifndef PD_STEREODELAY_H
#define PD_STEREODELAY_H

#include "AudioEffect.h"

class StereoDelay : public AudioEffect{
public:
    float delayTime = 0;
    int position = 0;

    StereoDelay(float delayTime_);

    void apply(float * lsample, float * rsample);

private:
    float buffer[500];
};


#endif //PD_STEREODELAY_H
