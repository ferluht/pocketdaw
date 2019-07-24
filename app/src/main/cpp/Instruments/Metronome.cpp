//
// Created by Admin on 12.06.2019.
//

#include "Metronome.h"

Metronome::Metronome(){
//    attach(new Text("Fonts/Roboto-Regular.ttf", L"attack\n", 0.3, 0.4));
//    attach(new Encoder("Textures/encoder.bmp", 0.6f, 0.5f, &A));
//    attach(new Text("Fonts/Roboto-Regular.ttf", L"decay\n", -0.2, 0.4));
//    attach(new Encoder("Textures/encoder.bmp", 0.1f, 0.5f, &D));
}

void Metronome::updateState(MetronomeState *state, MidiData md){
    if (md.data2 != 0) {
        state->ad.A = (int)(A*2000);
        state->ad.D = (int)(D*2000);
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