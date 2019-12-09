//
// Created by Admin on 13.06.2019.
//

#include <GUI/Button.h>
#include "Delay.h"

Delay::Delay() : AudioEffect(L"Delau")
{
    setRatio(0.45f);

    delay_time = new Encoder(L"time", -1, [this](float value) {}, 1);
    delay_time->place(0.25, 0.1);
    delay_time->setHeight(0.25);
    GAttach(delay_time);
    MConnect(delay_time);

    feedback = new Encoder(L"feedback", -1, [this](float value) {}, 2);
    feedback->place(0.25, 0.4);
    feedback->setHeight(0.25);
    GAttach(feedback);
    MConnect(feedback);

    drywet = new Encoder(L"dru/wet", -1, [this](float value) {}, 3);
    drywet->place(0.25, 0.7);
    drywet->setHeight(0.25);
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
    if (*isOn) {
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
    return *isOn;
}