//
// Created by Admin on 12.06.2019.
//

#include "Metronome.h"
#include "GUI/Text.h"
#include "GUI/Encoder.h"

Metronome::Metronome(){
//    attach(new Text("Fonts/Roboto-Regular.ttf", L"attack\n", 0.3, 0.4));
//    attach(new Encoder("Textures/encoder.bmp", 0.6f, 0.5f, &A));
//    attach(new Text("Fonts/Roboto-Regular.ttf", L"decay\n", -0.2, 0.4));
    attach(new Text("Fonts/Roboto-Regular.ttf", L"metronome", 0.05, 0.05, 2, 0.07));

    attach(new Encoder(0, 0.1, 0.2, 2, 0.2, "Textures/encoder.bmp", [this](float state){}));
    attach(new Text("Fonts/Roboto-Regular.ttf", L"level", 0.12, 0.4, 2, 0.065));

    attach(new Encoder(0, 0.4, 0.2, 2, 0.2, "Textures/encoder.bmp", [this](float state){}));
    attach(new Text("Fonts/Roboto-Regular.ttf", L"level", 0.42, 0.4, 2, 0.065));

    attach(new Encoder(0, 0.7, 0.2, 2, 0.2, "Textures/encoder.bmp", [this](float state){}));
    attach(new Text("Fonts/Roboto-Regular.ttf", L"level", 0.72, 0.4, 2, 0.065));
//    attach(new Encoder("Textures/encoder.bmp", 0.1f, 0.5f, &D));
}

void Metronome::updateState(MetronomeState *state, MData md){
    if (md.data2 != 0) {
        state->ad.A = (int)(A*2000);
        state->ad.D = (int)(D*20000);
        state->phase2 = 0;
//        state->beat = md.beat;
        state->note = md.data1;
        state->volume = md.data2 / 127.0;
        state->phase_increment = -1;
        state->ad.attack();
        state->setActive(true);
    }else{
        state->ad.release();
    }
}

void Metronome::render(MetronomeState * state, double beat, float * lsample, float * rsample)
{
    if (state->phase_increment < 0){
        state->phase_increment = getPhaseIncrement(state->note);
    }
    float sample = sin(state->phase);// * sin(state->phase2) + cnt;
    cnt += cnt;
    if (cnt > 0.8) cnt /= 10000;
//    float sample = sin((beat - state->beat)*256*M_PI);
    sample = state->ad.apply(sample) * state->volume;
    //state->volume = 0.5 + 0.3*(sin(state->phase/73));
    if (!state->ad.active) {
        state->setActive(false);
    }
    state->phase += state->phase_increment;
    state->phase2 += state->phase_increment*0.4;
    state->phase_increment += sin(state->phase) * 0.001 + sin(state->phase2) * 0.0005;

    *lsample = sample;
    *rsample = sample;
}