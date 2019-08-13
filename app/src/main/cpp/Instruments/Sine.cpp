//
// Created by Admin on 09.08.2019.
//

#include "Sine.h"

Sine::Sine(){
    GAttachTexture("Textures/effect_canvas.bmp");

    enc = new Encoder(L"aaa", 0, [this](float value){k = (value + 1)*5 + 2;});
    enc->place(0.2, 0.2, 0.4, 0.4);
    GAttach(enc);
    MConnect(enc);
}

void Sine::updateState(SineState *state, MData md){
    if (md.data2 != 0) {
        state->note = md.data1;
        state->phase_increment = getPhaseIncrement(state->note);
        state->phase = 0;
        state->phase2 = 0;
        state->setActive(true);
    }else{
        state->setActive(false);
    }
}

void Sine::render(SineState * state, double beat, float * lsample, float * rsample)
{
    float sample = sin(state->phase) * sin(state->phase2); //(sin(fmod(beat, M_PI)) + 1)/4; //* sin(state->phase2); //(sin(fmod(beat, M_PI)) + 1)/4;// * sin(state->phase2) + cnt;
    state->phase += state->phase_increment;
    state->phase2 += state->phase_increment / k;

    *lsample = sample;
    *rsample = sample;
}