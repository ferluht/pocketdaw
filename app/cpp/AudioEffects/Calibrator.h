//
// Created by ferluht on 13/09/2020.
//

#ifndef PD_CALIBRATOR_H
#define PD_CALIBRATOR_H


#include <GUI/Button.h>
#include "AudioEffect.h"

class Calibrator : public AudioEffect {

    const int window_size = 100;

    int deltatime = 0;

    int lag_counter = 0;

    GUI::Text * lag;

    const int initial_period = 20000;
    const int initial_deltatime = 30;
    int period = initial_period;
    int time_since_last_spike = initial_period;

    float * window;
    int wpos;
    float mean, std;

    void meanstd();

    GUI::Button * en;

public:

    Calibrator();

    bool ARender(double beat, float * lsample, float * rsample) override;

};


#endif //PD_CALIBRATOR_H
