//
// Created by Admin on 08.06.2019.
//

#ifndef PD_OSCILLATOR_H
#define PD_OSCILLATOR_H

#include <atomic>
#include <stdint.h>
#include "Module.h"

enum {
    VIN, PHASE, AMP, VOUT
};

class VCO : public Module {
public:
    VCO();
    void step();

private:

    float phaseIncrement_ = 2.0;
};

#endif //PD_OSCILLATOR_H
