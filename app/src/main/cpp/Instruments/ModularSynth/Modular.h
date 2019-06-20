//
// Created by Admin on 09.06.2019.
//

#ifndef PD_MODULAR_H
#define PD_MODULAR_H

#include <stdlib.h>
#include <list>
#include "Module.h"

class Modular {

public:
    //Module * list;

    std::list<Module*> Modules;

    Parameter vout;

    float master_amp = 0;

    float note = 0, rate = 0;

    Modular();
    void add(Module* mod);
    float step();
    void setWaveOn(bool isWaveOn, float x, float y);
    void render(float *audioData, int32_t numFrames);

private:
    std::atomic<bool> isWaveOn_{false};
    double phase_ = 0.0;
    double phaseIncrement_ = 0.01;
};


#endif //PD_MODULAR_H
