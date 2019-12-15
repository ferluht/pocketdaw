//
// Created by ibelikov on 13.12.19.
//

#include "Snare.h"

Snare::Snare() : Instrument<SnareState>(1, "SnareDrum") {
    shape->setRatio(0.4);

    rng = new std::mt19937(dev());
    dist = new std::uniform_real_distribution<float>(-1, 1);

    attack = new GUI::Encoder("attack", 0, 1, 0, 1);
    attack->shape->lPlace({0.05, 0.01});
    attack->shape->lSetHeight(0.2);
    GAttach(attack);
    MConnect(attack);

    tone = new GUI::Encoder("tone", 0, 2, 0, 10);
    tone->shape->lPlace({0.05, 0.25});
    tone->shape->lSetHeight(0.2);
    GAttach(tone);
    MConnect(tone);

    sweep_time = new GUI::Encoder("sweep", 0, 3, 0, 1);
    sweep_time->shape->lPlace({0.05, 0.5});
    sweep_time->shape->lSetHeight(0.2);
    GAttach(sweep_time);
    MConnect(sweep_time);

    decay = new GUI::Encoder("decay", 0.05, 3, 0.05, 1);
    decay->shape->lPlace({0.55, 0.5});
    decay->shape->lSetHeight(0.2);
    GAttach(decay);
    MConnect(decay);

    waveform = new GUI::Encoder("shape", 0, 1, 0, 1);
    waveform->shape->lPlace({0.55, 0.01});
    waveform->shape->lSetHeight(0.2);
    GAttach(waveform);
    MConnect(waveform);

    sweep_amt = new GUI::Encoder("sweep lvl", 0, 2, 0, 48);
    sweep_amt->shape->lPlace({0.55, 0.25});
    sweep_amt->shape->lSetHeight(0.2);
    GAttach(sweep_amt);
    MConnect(sweep_amt);

    trig = new GUI::TapButton("snare", [this] (bool state) {triggered = true;});
    trig->shape->lPlace({0.05, 0.75});
    trig->shape->lSetHeight(0.2);
    trig->shape->lSetWidth(0.9);
    GAttach(trig);
    MConnect(trig);
}

void Snare::MRender(double beat) {
    if (triggered) {
        MIn({beat, NOTEON_HEADER, 62, 100});
        triggered = false;
    }
}

void Snare::IUpdateState(SnareState *state, MData md) {
    if (((md.status & 0xF0) == NOTEON_HEADER) && (md.data2 != 0)) {
        state->note = 0;
        state->volume = (float)md.data2/127.f;
        if (!state->isActive()) state->phase = -2*M_PI;
        state->ad.attack(md.beat);
        state->sweep_env.attack(md.beat);
        state->setActive(true);
    }
}

void Snare::IARender(SnareState *state, double beat, float *lsample, float *rsample) {
    if (state->phase > M_PI) state->phase -= 2*M_PI;

    float output = osc(state->phase) * state->volume;

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