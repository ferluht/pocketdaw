//
// Created by Admin on 12.06.2019.
//

#ifndef PD_INSTRUMENT_H
#define PD_INSTRUMENT_H

#include "../Orchestration/Midi.h"

class Instrument {
public:
    Instrument();
    virtual void midiCommand(MidiData md);
    virtual float render();
};


#endif //PD_INSTRUMENT_H
