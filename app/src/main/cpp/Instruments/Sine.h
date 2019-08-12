//
// Created by Admin on 09.08.2019.
//

#ifndef PD_SINE_H
#define PD_SINE_H

#include "Instruments/Instrument.h"
#include <GUI/Encoder.h>

class SineState : public InstrumentState{
public:
    float phase2;
};

class Sine : public Instrument<SineState>{

    Encoder * enc;

    float k;

public:

    Sine();

    void updateState(SineState * state, MData md) override;

    void render(SineState * state, double beat, float * lsample, float * rsample) override ;

    void MIn(MData cmd) override ;
};


#endif //PD_SINE_H
