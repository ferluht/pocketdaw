//
// Created by Admin on 13.08.2019.
//

#include "Operator.h"

Operator::Operator(){
    GAttachTexture("Textures/effect_canvas.bmp");

    for (int i = 0; i < 4; i++) {
        enc_ratios[i] = new Encoder(L"course 1", 0, [this, i](float value) {
            ratios[i] = (int) (10 * (value + 1));
            if (ratios[i] == 0) ratios[i] = 0.5;
        }, i + 1);
        enc_ratios[i]->place(0.03, 0.03+0.22*i, 0.2, 0.2);
        GAttach(enc_ratios[i]);
        MConnect(enc_ratios[i]);
    }

    for (int i = 0; i < 4; i++) {
        enc_levels[i] = new Encoder(L"level 1", -1, [this, i](float value) {
            levels[i] = (value + 1)/2;
        }, i + 5);
        enc_levels[i]->place(0.33, 0.03+0.22*i, 0.2, 0.2);
        GAttach(enc_levels[i]);
        MConnect(enc_levels[i]);
    }
}

void Operator::updateState(OperatorState *state, MData md){
    if (md.data2 != 0) {
        state->note = md.data1;
        state->phase_increment = getPhaseIncrement(state->note);
        for (int i = 0; i < 4; i ++) state->phases[i] = 0;
        state->setActive(true);
    }else{
        state->setActive(false);
    }
}

void Operator::render(OperatorState * state, double beat, float * lsample, float * rsample)
{
    float sample = sin(state->phases[0]) * levels[0] + \
                   sin(state->phases[1]) * levels[1] + \
                   sin(state->phases[2]) * levels[2] * sin(state->phases[3]) * levels[3];
    sample /= 3;
    for (int i = 0; i < 4; i ++) state->phases[i] += state->phase_increment * ratios[i];

    *lsample += sample;
    *rsample += sample;
}