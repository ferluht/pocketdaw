//
// Created by Admin on 12.06.2019.
//

#include "Metronome.h"
#include <cmath>

void Metronome::updateState(MetronomeState *state, MidiData md){
    if (md.data2 != 0) {
        state->ad.A = (int)(A*5000);
        state->phase2 = 0;
        state->beat = md.beat;
        state->note = md.data1;
        state->volume = md.data2 / 127.0;
        state->phase_increment = -1;
        state->ad.attack();
        state->setActive(true);
    }else{
        state->ad.release();
    }
}

float Metronome::render(MetronomeState * state, double beat)
{
    if (state->phase_increment < 0){
        state->phase_increment = getPhaseIncrement(state->note);
    }
    float sample = sin(state->phase) * sin(state->phase2);
//    float sample = sin((beat - state->beat)*256*M_PI);
    sample = state->ad.apply(sample) * state->volume;
    state->volume = 0.5 + 0.3*(sin(state->phase/73));
    if (!state->ad.active) {
        state->setActive(false);
    }
    state->phase += state->phase_increment;
    state->phase2 += state->phase_increment*0.4;
    state->phase_increment += sin(state->phase) * 0.001 + sin(state->phase2) * 0.0005;
    return sample;
}