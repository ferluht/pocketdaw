//
// Created by Admin on 25.07.2019.
//

#include "StereoDelay.h"
#include <GUI/Button.h>

StereoDelay::StereoDelay(float delayTime_) : AudioEffect(300,500,200,200){

//    attach(new Text("Fonts/Roboto-Regular.ttf", L"feedback\n", 0.3, 0.1));
//    attach(new Encoder("Textures/encoder.bmp", 0.6f, 0.2f, &feedback));
//    attach(new Text("Fonts/Roboto-Regular.ttf", L"delay time\n", -0.2, 0.1));
//    attach(new Encoder("Textures/encoder.bmp", 0.1f, 0.2f, &delayTime));

    attach(new Button(400, 600, 100, 100, "Textures/encoder.bmp", [this](bool state){this->isOn = state;}));

    for (int i = 0; i < 500; i ++){
        buffer[i] = 0;
    }
    delayTime = delayTime_;
    position = 0;
}

void StereoDelay::apply(float * lsample, float * rsample)
{
    if (isOn) {
        int del_position = position - (int) (delayTime * 500);
        buffer[position] = *rsample;
        if (del_position < 0) del_position += 500;
        *rsample = buffer[del_position];
        position = (position + 1) % 500;
    }
}