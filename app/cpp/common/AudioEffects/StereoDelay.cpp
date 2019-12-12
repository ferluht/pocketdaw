//
// Created by Admin on 25.07.2019.
//

#include "StereoDelay.h"
#include <GUI/Button.h>
#include <GUI/Encoder.h>

StereoDelay::StereoDelay() : AudioEffect("Stereo delay"){

    delay_time = new GUI::Encoder("size", -1, [this](float value) {
                this->delayTime = value/2 + 0.5f;
                if (this->delayTime < 0.01f) this->delayTime = 0.01f;
                if (this->delayTime > 0.99f) this->delayTime = 0.99f;
        }, 1);
    delay_time->shape->lPlace({0.25f, 0.1f});
    delay_time->shape->lSetHeight(0.25f);
    GAttach(delay_time);
    MConnect(delay_time);

    for (float & i : buffer) i = 0;
    delayTime = 0;
    position = 0;
}

void StereoDelay::apply(float * lsample, float * rsample)
{
    if (enabled()) {
        int del_position = position - (int) (delayTime * 500);
        buffer[position] = *rsample;
        if (del_position < 0) del_position += 500;
        *rsample = buffer[del_position];
        position = (position + 1) % 500;
    }
}