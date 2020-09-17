//
// Created by Admin on 08.06.2019.
//

#include "VCO.h"
#include <math.h>

VCO::VCO() : Module(){
    params = new Parameter[10];

    params[VIN].name = "VIN";
    params[VIN].value = 0.002;
    params[AMP].name = "AMP";
    params[AMP].value = 0.8;
    params[PHASE].name = "PHASE";
    params[PHASE].value = 0;

}

void VCO::step() {

    float vin = params[VIN].get();
    float phase = params[PHASE].get();
    float amp = params[AMP].get();

    params[VOUT].set(sin(phase) * amp);

    // if (!param[PHASE]->ismapped())
    params[PHASE].setdelta(vin * phaseIncrement_);

//    params
//
//    (float) (sin(phase_) * AMPLITUDE);
//
//    // Increments the phase, handling wrap around.
//    phase_ += phaseIncrement_;
    if (params[PHASE].value > 6.28) params[PHASE].setdelta(-6.28);
}