//
// Created by ferluht on 13/04/2020.
//

#ifndef PD_VCO_H
#define PD_VCO_H


#include "Instrument.h"

class VCOState : public InstrumentState {

public:

    double beat;
    float volume;
    double frequency;
    double phase;
    double phase_increment;

};

class VCO : public Instrument<InstrumentState> {

public:

    VCO() : Instrument<VCOState> (1, "VCO"){

    }

};


#endif //PD_VCO_H
