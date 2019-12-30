//
// Created by ibelikov on 12.12.19.
//

#include "Kick.h"

Kick::Kick() : AnalogDrum<KickState>("Kick") {

    attack = new GUI::Encoder("attack", 0, 0, 1);
    placeEncoder(attack, 0, 0);

    tone = new GUI::Encoder("tone", 0, 0, 10);
    placeEncoder(tone, 0, 1);

    sweep_time = new GUI::Encoder("sweep", 0, 0, 1);
    placeEncoder(sweep_time, 0, 2);

    decay = new GUI::Encoder("decay", 0.05, 0.05, 1);
    placeEncoder(decay, 1, 2);

    waveform = new GUI::Encoder("shape", 0, 0, 1);
    placeEncoder(waveform, 1, 0);

    sweep_amt = new GUI::Encoder("sweep lvl", 0, 0, 48);
    placeEncoder(sweep_amt, 1, 1);

    jack = new GUI::Jack(GUI::Jack::INPUT);
    jack->GPlace({0.2, 0.4});
    jack->GSetHeight(0.4);
    GAttach(jack);
}

void Kick::IUpdateState(KickState *state, MData md) {
    AnalogDrum::IUpdateState(state, md);
    if (((md.status & 0xF0) == NOTEON_HEADER) && (md.data2 != 0)) {
        state->note = 0;
        state->volume = (float)md.data2/127.f;
        if (!state->isActive()) state->phase = -2*M_PI;
        state->ad.attack(md.beat);
        state->sweep_env.attack(md.beat);
        state->setActive(true);
    }
}

void Kick::IARender(KickState *state, double beat, float *lsample, float *rsample) {
    if (state->phase > M_PI) state->phase -= 2*M_PI;

    float output = osc(state->phase) * state->volume;

    *tone = jack->value + 1;

    state->ad.A = (*attack) * (*attack);
    state->ad.D = (*decay) * (*decay);
    if (!state->ad.ARender(beat, &output, &output)) state->setActive(false);

    float sweep_level = 1;
    state->sweep_env.D = *sweep_time;
    state->sweep_env.ARender(beat, &sweep_level, &sweep_level);

    state->phase += getPhaseIncrement(12*(*tone) + sweep_level * *sweep_amt);

    *lsample += output;
    *rsample += output;
}