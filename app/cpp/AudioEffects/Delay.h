//
// Created by Admin on 13.06.2019.
//

#ifndef PD_DELAY_H
#define PD_DELAY_H

#include <GUI/Encoder.h>
#include <GUI/Led.h>
#include "AudioEffect.h"

class Delay : public AudioEffect{

    const int buffer_size = 10*48000;

    int position = 0;

    float * buffer;

    GUI::Encoder * delay_time;
    GUI::Encoder * feedback;
    GUI::Encoder * drywet;

    GUI::Led * error_led;

public:

    Delay();

    bool ARender(double beat, float * lsample, float * rsample) override;
};


#endif //PD_DELAY_H
