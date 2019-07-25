//
// Created by Admin on 13.06.2019.
//

#ifndef PD_DELAY_H
#define PD_DELAY_H

#include "AudioEffect.h"

class Delay : public AudioEffect{
public:
    float delayTime = 0;
    int position = 0;
    float feedback = 0.4;

    Delay(float delayTime_, float feedback_);

    void apply(float * lsample, float * rsample);

private:
    float buffer[50000];
};


#endif //PD_DELAY_H
