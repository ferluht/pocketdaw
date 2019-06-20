//
// Created by Admin on 12.06.2019.
//

#ifndef PD_METRONOME_H
#define PD_METRONOME_H

#include "Instrument.h"
#include "ADSR.h"

class Metronome : public Instrument{

public:

    int duration;

    float phase = 0, phase_inc = 0.02, vol;

    ADSR ad;

    Metronome();
    void midiCommand(MidiData md);
    float render();
};


#endif //PD_METRONOME_H
