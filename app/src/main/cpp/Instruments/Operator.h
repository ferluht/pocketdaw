//
// Created by Admin on 13.08.2019.
//

#ifndef PD_OPERATOR_H
#define PD_OPERATOR_H

#include "Instruments/Instrument.h"
#include <GUI/Encoder.h>

class OperatorState : public InstrumentState{
public:
    double beat;
    unsigned char velocity;

    float volume;
    double phases[4];
    double frequency;
    double phase_increment;
};

class Operator : public Instrument<OperatorState>{

    Encoder * enc_ratios[4];
    Encoder * enc_levels[4];

    float k;

    double phase_shifts[4];
    float ratios[4];
    float levels[4];

public:

    Operator();

    void updateState(OperatorState * state, MData md) override;

    void render(OperatorState * state, double beat, float * lsample, float * rsample) override ;
};



#endif //PD_OPERATOR_H
