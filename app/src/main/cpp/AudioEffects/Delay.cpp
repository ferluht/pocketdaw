//
// Created by Admin on 13.06.2019.
//

#include "Delay.h"

Delay::Delay(float delayTime_, float feedback_) : AudioEffect()
{
    for (int i = 0; i < 50000; i ++){
        buffer[i] = 0;
    }
    delayTime = delayTime_;
    feedback = feedback_;
}

float Delay::apply(float sample)
{
    int index = (int)(delayTime*50000) - position;
    sample += buffer[index];
    buffer[index] = sample*feedback;
    position = (position + 1)%((int)(delayTime*50000));
    return sample;
}