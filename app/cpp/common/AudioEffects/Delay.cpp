//
// Created by Admin on 13.06.2019.
//

#include <GUI/Button.h>
#include "Delay.h"

Delay::Delay() : AudioEffect("Delay")
{
    shape->setRatio(0.45f);

    delay_time = new GUI::Encoder("time", -1, [this](float value) {}, 1);
    delay_time->shape->lPlace({0.25, 0.1});
    delay_time->shape->lSetHeight(0.25);
    GAttach(delay_time);
    MConnect(delay_time);

    feedback = new GUI::Encoder("feedback", -1, [this](float value) {}, 2);
    feedback->shape->lPlace({0.25, 0.4});
    feedback->shape->lSetHeight(0.25);
    GAttach(feedback);
    MConnect(feedback);

    drywet = new GUI::Encoder("dry/wet", -1, [this](float value) {}, 3);
    drywet->shape->lPlace({0.25, 0.7});
    drywet->shape->lSetHeight(0.25);
    GAttach(drywet);
    MConnect(drywet);

    buffer = new float[buffer_size];
    for (int i = 0; i < buffer_size; i ++) buffer[i] = 0;
    *delay_time = 0;
    *feedback = 0;
    position = 0;
}

bool Delay::ARender(double beat, float *lsample, float *rsample){
    float sample = (*lsample + *rsample)/2;
    if (enabled()) {
        int window_size = (int)((*delay_time + 1)/2 * buffer_size);
        if (window_size == 0) return false;
        int index = window_size - position;
        if (index < 0) index += window_size;
        sample += buffer[index];
        buffer[index] = sample * (*feedback);
        position = (position + 1) % window_size;
    }

    float prop = (*drywet + 1)/2;

    *lsample = sample*prop + *lsample * (1-prop);
    *rsample = sample*prop + *rsample * (1-prop);
    return enabled();
}