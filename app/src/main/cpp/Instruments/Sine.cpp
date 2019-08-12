//
// Created by Admin on 09.08.2019.
//

#include "Sine.h"

Sine::Sine(){
    GAttachTexture("Textures/effect_canvas.bmp");

    k = 300;
    enc = new Encoder(0, [](float a){});
    enc->place(0.2, 0.2, 0.4, 0.4);
    GAttach(enc);
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
    float sample = sin(state->phase) * sin(state->phase2); //(sin(fmod(beat, M_PI)) + 1)/4;// * sin(state->phase2) + cnt;
    state->phase += state->phase_increment;
    state->phase2 += state->phase_increment / k;

    *lsample = sample;
    *rsample = sample;
}

void Sine::MIn(MData cmd) {
    Instrument::MIn(cmd);
    if (cmd.status == 0xB0) {
        enc->angle = cmd.data2 / 127.f - 0.5f;
        k =  cmd.data2 * 5 + 1;
    }
}