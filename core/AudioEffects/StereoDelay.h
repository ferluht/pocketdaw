//
// Created by Admin on 25.07.2019.
//

#ifndef PD_STEREODELAY_H
#define PD_STEREODELAY_H

#include <GUI/Encoder.h>
#include "AudioEffect.h"

class StereoDelay : public AudioEffect{
public:
    float delayTime = 0;
    int position = 0;
    GUI::Encoder * delay_time;

    StereoDelay();

    bool ARender(double beat, float * lsample, float * rsample) override ;

private:
    float buffer[500];
};


#endif //PD_STEREODELAY_H
