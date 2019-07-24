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

    float A = 0.1, D = 0.5, S = 0.5, R = 0.3;

    Metronome();

    void updateState(MetronomeState * state, MidiData md) override;
    float render(MetronomeState * state, double beat) override ;
};


#endif //PD_METRONOME_H
