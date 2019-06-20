//
// Created by Admin on 13.06.2019.
//

#ifndef PD_DELAY_H
#define PD_DELAY_H

#include "AudioEffect.h"

class Delay : public AudioEffect{
public:
    int delayTime = 1000;
    int position = 0;
    float feedback = 0.4;

    Delay(int delayTime_, float feedback_);
    float apply(float sample);

private:
    float buffer[50000];
};


#endif //PD_DELAY_H
