//
// Created by Admin on 12.06.2019.
//

#ifndef PD_METRONOME_H
#define PD_METRONOME_H

#include "Instrument.h"
#include "ADSR.h"

class MetronomeState : public InstrumentState{
public:

    int duration;
    float vol;
    ADSR ad;

    MetronomeState(unsigned char note, unsigned char velocity);
};

class Metronome : public Instrument<MetronomeState>{

public:

//    Metronome();
    float render(MetronomeState * state) override ;
};


#endif //PD_METRONOME_H
