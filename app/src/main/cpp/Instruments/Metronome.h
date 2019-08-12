//
// Created by Admin on 12.06.2019.
//

#ifndef PD_METRONOME_H
#define PD_METRONOME_H

#include "Instrument.h"
#include "ADSR.h"
#include <cmath>

class MetronomeState : public InstrumentState{
public:

    ADSR ad;

    double phase2;
};

class Metronome : public Instrument<MetronomeState>{

public:

    float A = 0.1, D = 0.2, S = 0.7, R = 0.1;
    float cnt = 0.001;

    Metronome();

    void updateState(MetronomeState * state, MData md) override;

    void render(MetronomeState * state, double beat, float * lsample, float * rsample) override ;
};


#endif //PD_METRONOME_H
