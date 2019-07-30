//
// Created by Admin on 25.07.2019.
//

#include "StereoDelay.h"
#include <GUI/Button.h>
#include <GUI/Text.h>
#include <GUI/Encoder.h>

StereoDelay::StereoDelay(float delayTime_) : AudioEffect(0.4,0.4,0.6,0.6){

//    attach(new Text("Fonts/Roboto-Regular.ttf", L"feedback\n", 0.3, 0.1));
//    attach(new Encoder("Textures/encoder.bmp", 0.6f, 0.2f, &feedback));
//    attach(new Text("Fonts/Roboto-Regular.ttf", L"delay time\n", -0.2, 0.1));
//    attach(new Encoder("Textures/encoder.bmp", 0.1f, 0.2f, &delayTime));

    attach(new Text("Fonts/Roboto-Regular.ttf", L"widener", 0.05, 0.05, 2, 0.07));

    attach(new Encoder(0, 0.27, 0.2, 2, 0.35, "Textures/encoder.bmp",
            [this](float state){
                this->delayTime = state/2 + 0.5;
                if (this->delayTime < 0.01) this->delayTime = 0.01;
                if (this->delayTime > 0.99) this->delayTime = 0.99;
    }));
    attach(new Text("Fonts/Roboto-Regular.ttf", L"size", 0.32, 0.6, 2, 0.1));

    attach(new Button(L"ON/OFF", 0.1, 0.8, 0.15, 0.6, "Textures/button.bmp",
            [this](bool state){
                this->isOn = state;
    }));

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