//
// Created by Admin on 12.06.2019.
//

#include "Metronome.h"
#include <cmath>

void MetronomeState::update(MidiData md)
{
    if (md.data2 != 0) {
        this->note = md.data1;
        vol = md.data2 / 127.0;
        phase_increment = -1;
        ad.attack();
    }else{
        ad.release();
    }
}

MetronomeState::MetronomeState(){
    note = 0;
    phase = 0;
    phase_increment = -1;
}

float Metronome::render(MetronomeState * state)
{
    if (state->phase_increment < 0){
        state->phase_increment = getPhaseIncrement(state->note);
    }
    float sample = sin(state->phase);
    sample = state->ad.apply(sample) * state->vol;
    if (!state->ad.active) {
        state->note = 0;
    }
    state->phase += state->phase_increment;
    return sample;
}