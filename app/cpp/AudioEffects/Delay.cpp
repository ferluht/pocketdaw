//
// Created by Admin on 13.06.2019.
//

#include <GUI/Button.h>
#include "Delay.h"

Delay::Delay() : AudioEffect("Delay")
{
    GSetRatio(0.45f);

    delay_time = new GUI::Encoder("time", -1, 0, buffer_size / AObject::sample_rate);
    delay_time->GPlace({0.25, 0.1});
    delay_time->GSetHeight(0.25);
    GAttach(delay_time);
    MConnect(delay_time);

    feedback = new GUI::Encoder("feedback", 0, 0, 1);
    feedback->GPlace({0.25, 0.4});
    feedback->GSetHeight(0.25);
    GAttach(feedback);
    MConnect(feedback);

    drywet = new GUI::Encoder("dry/wet", -1);
    drywet->GPlace({0.25, 0.7});
    drywet->GSetHeight(0.25);
    GAttach(drywet);
    MConnect(drywet);

    error_led = new GUI::Led(false);
    error_led->GPlace({0.05, 0.05});
    error_led->GSetHeight(0.05);
    error_led->GSetRatio(1);
    GAttach(error_led);

    buffer = new float[buffer_size];
    for (int i = 0; i < buffer_size; i ++) buffer[i] = 0;
    *delay_time = 0;
    *feedback = 0;
    position = 0;
}

bool Delay::ARender(double beat, float *lsample, float *rsample){
    float sample = (*lsample + *rsample)/2;
    if (enabled()) {
        int delay_size = (int)(*delay_time * AObject::sample_rate);

        position = position % delay_size;

        if (delay_size <= AObject::latency){
            error_led->on();
            return false;
        }
        error_led->off();

        if (delay_size == 0) return false;

        if (delay_size * 2 > buffer_size) return false;

        int index = (position + 1 + (int)AObject::latency) % delay_size;
//        if (index < 0) index = delay_size - index;

        buffer[position] = (buffer[position] + sample) * (*feedback);
        sample = buffer[index];

        position = (position + 1) % delay_size;

        *lsample = sample;
        *rsample = sample;
    } else {
        *lsample = 0;
        *rsample = 0;
    }

//    float prop = (*drywet + 1)/2;

//    *lsample = sample*prop + *lsample * (1-prop);
//    *rsample = sample*prop + *rsample * (1-prop);
    return enabled();
}