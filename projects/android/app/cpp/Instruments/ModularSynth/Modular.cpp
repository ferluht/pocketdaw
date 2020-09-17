//
// Created by Admin on 09.06.2019.
//

#include <math.h>
#include "Modular.h"
#include "VCO.h"


Modular::Modular() {
    VCO * vco1 = new VCO();
    add(vco1);
    vco1->params[AMP].set(0.5);
    vco1->params[VIN].set(0);
    vco1->params[VIN].map(&note);
    vout.map(&(vco1->params[VOUT].value));

    VCO * vco2 = new VCO();
    add(vco2);
    vco2->params[AMP].set(0.4);
    vco2->params[VIN].set(0);
    vco1->params[AMP].map(&vco2->params[VOUT].value);
    vco2->params[VIN].map(&rate);
    note = 0;


    VCO * vco3 = new VCO();
    add(vco3);
    vco3->params[AMP].set(0.4);
    vco3->params[VIN].set(0.073);
    vco1->params[PHASE].map(&vco3->params[VOUT].value);
    vco3->params[VIN].map(&note);
    note = 0;
}

void Modular::setWaveOn(bool isWaveOn, float x, float y) {
    isWaveOn_.store(isWaveOn);
    note = 1 - exp(-y/50000);
    rate = x/1500000;
}

void Modular::render(float *audioData, int32_t numFrames) {
    for (int i = 0; i < numFrames; i++) {
        if (isWaveOn_.load()) {
            // Calculates the next sample value for the sine wave.
            audioData[i] = step() * master_amp;
            if (master_amp < 0.95) master_amp += 0.05;
        } else {
            // Outputs silence by setting sample value to zero.
            master_amp = 0;
            audioData[i] = 0;
        }
    }
}

float Modular::step() {
    for (auto const& mod : Modules) {
        mod->step();
    }
    return vout.get();
}

void Modular::add(Module *mod) {
    Modules.push_front(mod);
}