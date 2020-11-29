//
// Created by ferluht on 29/11/2020.
//

#ifndef ANDROID_PITCHSHIFTER_H
#define ANDROID_PITCHSHIFTER_H


#include "AudioEffect.h"

class PitchShifter : public AudioEffect {

    float mem_l = 0, mem_r = 0;

public:

    PitchShifter() : AudioEffect("Pitch Shifter") {

    }

    bool ARender(double beat, float * lsample, float * rsample) override  {

        *lsample = (*lsample + mem_l) / 2;
        *rsample = (*rsample + mem_r) / 2;

        mem_l = *lsample;
        mem_r = *rsample;

        return true;
    }

};


#endif //ANDROID_PITCHSHIFTER_H
