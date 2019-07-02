//
// Created by Admin on 12.06.2019.
//

#ifndef PD_METRONOME_H
#define PD_METRONOME_H

#include "Instrument.h"
#include "ADSR.h"

class MetronomeState : public InstrumentState{
public:

    ADSR ad;

    double phase2;

    void update(MidiData md) override;
};

class Metronome : public Instrument<MetronomeState>{

public:

    float render(MetronomeState * state, double beat) override ;
};


#endif //PD_METRONOME_H
