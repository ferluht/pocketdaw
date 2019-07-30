//
// Created by Admin on 13.06.2019.
//

#include <GUI/Button.h>
#include "Delay.h"

Delay::Delay(float delayTime_, float feedback_) : AudioEffect(300,500,200,200)
{

//    attach(new Text("Fonts/Roboto-Regular.ttf", L"feedback\n", 0.3, 0.1));
//    attach(new Encoder("Textures/encoder.bmp", 0.6f, 0.2f, &feedback));
//    attach(new Text("Fonts/Roboto-Regular.ttf", L"delay time\n", -0.2, 0.1));
//    attach(new Encoder("Textures/encoder.bmp", 0.1f, 0.2f, &delayTime));

    attach(new Button(L"ON/OFF", 400, 600, 100, 100, "Textures/button.bmp", [this](bool state){this->isOn = state;}));

    for (int i = 0; i < 50000; i ++){
        buffer[i] = 0;
    }
    delayTime = delayTime_;
    feedback = feedback_;
    position = 0;
}

void Delay::apply(float * lsample, float * rsample)
{
    float sample = (*lsample + *rsample)/2;
    if (isOn) {
        int index = (int) (delayTime * 50000) - position;
        sample += buffer[index];
        buffer[index] = sample * feedback;
        position = (position + 1) % ((int) (delayTime * 50000));
    }
    *lsample = sample;
    *rsample = sample;
}