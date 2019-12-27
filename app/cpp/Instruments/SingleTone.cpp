//
// Created by Admin on 13.09.2019.
//

#include "SingleTone.h"

SingleTone::SingleTone() : Instrument<SingleToneState>(1, "ST"){

    GSetRatio(1);

    interp1 = new GUI::Encoder("waveform1", 0, [this](float value) {}, 1, 0, 1);
    interp1->GPlace({encoder_spacing, row_1st});
    interp1->GSetHeight(encoder_height);
    GAttach(interp1);
    MConnect(interp1);

    interp2 = new GUI::Encoder("waveform2", 0, [this](float value) {}, 2, 0, 1);
    interp2->GPlace({encoder_spacing + encoder_height, row_1st});
    interp2->GSetHeight(encoder_height);
    GAttach(interp2);
    MConnect(interp2);

    A = new GUI::Encoder("A", 0, [this](float value) {}, 3, 0, 1);
    A->GPlace({encoder_spacing, row_1st + encoder_height});
    A->GSetHeight(encoder_height);
    GAttach(A);
    MConnect(A);

    D = new GUI::Encoder("D", 0, [this](float value) {}, 4, 0, 1);
    D->GPlace({encoder_spacing + encoder_height, row_1st + encoder_height});
    D->GSetHeight(encoder_height);
    GAttach(D);
    MConnect(D);

    S = new GUI::Encoder("S", 0, [this](float value) {}, 5, 0, 1);
    S->GPlace({encoder_spacing + encoder_height * 2, row_1st + encoder_height});
    S->GSetHeight(encoder_height);
    GAttach(S);
    MConnect(S);

    R = new GUI::Encoder("R", 0, [this](float value) {}, 6, 0, 1);
    R->GPlace({encoder_spacing + encoder_height * 3, row_1st + encoder_height});
    R->GSetHeight(encoder_height);
    GAttach(R);
    MConnect(R);

    crossmod = new GUI::Encoder("crossmod", 0, [this](float value) {}, 7, 0, 1);
    crossmod->GPlace({encoder_spacing, row_1st + 2 * encoder_height});
    crossmod->GSetHeight(encoder_height);
    GAttach(crossmod);
    MConnect(crossmod);

    *A = 0.01;
    *D = 0.05;
    *S = 0.4;
    *R = 0.1;

    *interp1 = 1.0;
    *interp2 = 0.7;

    glide_time = sample_rate*0.1;
}

void SingleTone::IUpdateState(SingleToneState *state, MData md) {
    if (((md.status & 0xF0) == NOTEON_HEADER) && (md.data2 != 0)) {
        if (!state->isActive()) {
            state->glide = 0;
            state->glide_inc = 0;
            state->glide_dir = 0;
            state->phase1 = 0;
            state->phase2 = 0;
            state->adsr.attack(md.beat);
        } else {
            state->glide = - md.data1 + state->note;
            state->glide_dir = state->glide / abs(state->glide);
            state->glide_inc = - state->glide / glide_time;
        }
        state->note = md.data1;
        state->volume = (float)md.data2/127.f;
        state->setActive(true);
    }else{
        if (md.data1 == state->note){
            state->adsr.release(md.beat);
        }
    }
}

void SingleTone::IARender(SingleToneState *state, double beat, float *lsample, float *rsample) {
    float first = sinf(state->phase1) * (*interp1) + (state->phase1 / 6.283f * 2 - 1) * (1 - (*interp1));
    float second = sinf(state->phase2) * (*interp2);

    if (state->phase2 / 6.283f > 0.5) second += 1 - (*interp2);
    else second -= 1 - (*interp2);
//    + (state->phase2 / 6.283f * 2 - 1) * (1 - (*interp2 + 1)/2);

    float sample = (first * (*crossmod) + first * second * (1 - (*crossmod))) * state->volume;

    state->adsr.A = (*A) * (*A);
    state->adsr.D = (*D) * (*D);
    state->adsr.S = (*S) * (*S);
    state->adsr.R = (*R) * (*R);
    if(!state->adsr.ARender(beat, &sample, &sample)) state->setActive(false);

    state->phase1 += getPhaseIncrement(state->note + state->glide);
    if (state->phase1 > 6.283f) state->phase1 -= 6.283f;

    state->phase2 += getPhaseIncrement(state->note + 12 + state->glide);
    if (state->phase2 > 6.283f) state->phase2 -= 6.283f;

    if (state->glide * state->glide_dir > 0)
        state->glide += state->glide_inc;

    *lsample += sample;
    *rsample += sample;
}