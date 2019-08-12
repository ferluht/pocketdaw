//
// Created by Admin on 12.06.2019.
//

#ifndef PD_MIDIEFFECT_H
#define PD_MIDIEFFECT_H

#include <stdlib.h>
#include <queue>
#include "Orchestration/Midi.h"

class MidiEffect {

public:

    MidiEffect() = default;

    virtual void apply(std::priority_queue<MData> * midiQueue, double beat) {};
};


#endif //PD_MIDIEFFECT_H
