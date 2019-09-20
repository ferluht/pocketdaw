//
// Created by Admin on 13.09.2019.
//

#include <GUI/Text.h>
#include "SingleTone.h"

SingleTone::SingleTone() : Instrument<SingleToneState>(1, L"ST"){

    setRatio(1);

    interp1 = new Encoder(L"waveform1", 0, [this](float value) {}, 1, 0, 1);
    interp1->place(encoder_spacing, row_1st);
    interp1->setHeight(encoder_height);
    GAttach(interp1);
    MConnect(interp1);

    interp2 = new Encoder(L"waveform2", 0, [this](float value) {}, 2, 0, 1);
    interp2->place(encoder_spacing + encoder_height, row_1st);
    interp2->setHeight(encoder_height);
    GAttach(interp2);
    MConnect(interp2);

    A = new Encoder(L"A", 0, [this](float value) {}, 3, 0, 1);
    A->place(encoder_spacing, row_1st + encoder_height);
    A->setHeight(encoder_height);
    GAttach(A);
    MConnect(A);

    D = new Encoder(L"D", 0, [this](float value) {}, 4, 0, 1);
    D->place(encoder_spacing + encoder_height, row_1st + encoder_height);
    D->setHeight(encoder_height);
    GAttach(D);
    MConnect(D);

    S = new Encoder(L"S", 0, [this](float value) {}, 5, 0, 1);
    S->place(encoder_spacing + encoder_height*2, row_1st + encoder_height);
    S->setHeight(encoder_height);
    GAttach(S);
    MConnect(S);

    R = new Encoder(L"R", 0, [this](float value) {}, 6, 0, 1);
    R->place(encoder_spacing + encoder_height*3, row_1st + encoder_height);
    R->setHeight(encoder_height);
    GAttach(R);
    MConnect(R);

    crossmod = new Encoder(L"crossmod", 0, [this](float value) {}, 7, 0, 1);
    crossmod->place(encoder_spacing, row_1st + 2*encoder_height);
    crossmod->setHeight(encoder_height);
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
    if (md.data2 != 0) {
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