//
// Created by Admin on 12.06.2019.
//

#include "Metronome.h"
#include <cmath>

MetronomeState::MetronomeState(unsigned char note, unsigned char velocity){
    if (velocity) {
        phase = 0;
        phase_increment = M_PI / 48000.0 * getFrequency(440.0, note);
        vol = velocity / 127.0;
        ad.attack();
    } else {
        ad.release();
    }
}

float Metronome::render(MetronomeState * state)
{
    float sample = sin(state->phase);
    sample = state->ad.apply(sample) * state->vol;
    state->phase += state->phase_increment;
    return sample;
}