//
// Created by Admin on 09.08.2019.
//

#include "Sine.h"

Sine::Sine(unsigned int num_voices) : Instrument<SineState>(num_voices){
    GAttachTexture("Textures/effect_canvas.bmp");

    setRatio(2);

    enc_coarse = new Encoder(L"coarse", -1, [this](float value) {
        ratio = (int) (10 * (value + 1)) - 1;
    }, 1);
    enc_coarse->place(0.025, 0.05);
    enc_coarse->setHeight(0.4);
    GAttach(enc_coarse);
    MConnect(enc_coarse);

    enc_fine = new Encoder(L"fine", -1, [this](float value) {
        fine = (value + 1)/2;
    }, 2);
    enc_fine->place(0.225, 0.05);
    enc_fine->setHeight(0.4);
    GAttach(enc_fine);
    MConnect(enc_fine);

    enc_level = new Encoder(L"level", -1, [this](float value) {
        level = (value + 1)/2;
    }, 3);
    enc_level->place(0.425, 0.05);
    enc_level->setHeight(0.4);
    GAttach(enc_level);
    MConnect(enc_level);

    enc_attack = new Encoder(L"attack", 0, [this](float value) {
        A = (value + 1)/10;
    }, 5);
    enc_attack->place(0.625, 0.05);
    enc_attack->setHeight(0.4);
    GAttach(enc_attack);
    MConnect(enc_attack);

    enc_decay = new Encoder(L"decay", 0, [this](float value) {
        D = (value + 1)*2;
    }, 6);
    enc_decay->place(0.025, 0.5);
    enc_decay->setHeight(0.4);
    GAttach(enc_decay);
    MConnect(enc_decay);

    enc_sustain = new Encoder(L"sustain", 0, [this](float value) {
        S = (value + 1)/2;
    }, 7);
    enc_sustain->place(0.225, 0.5);
    enc_sustain->setHeight(0.4);
    GAttach(enc_sustain);
    MConnect(enc_sustain);

    enc_release = new Encoder(L"release", 0, [this](float value) {
        R = (value + 1)*5;
    }, 8);
    enc_release->place(0.425, 0.5);
    enc_release->setHeight(0.4);
    GAttach(enc_release);
    MConnect(enc_release);

    A = 0.02;
    D = 1.2;
    S = 0.7;
    R = 0.1;

//    graph = new TimeGraph(200);
//    graph->place(0.7, 0.05, 0.4, 0.4);
//    GAttach(graph);
//    graph_phase = 0;
}

void Sine::IUpdateState(SineState *state, MData md){
    if (md.data2 != 0) {
        state->note = md.data1;
        state->volume = (float)md.data2/127.f;
        if (!state->active) state->phase = 0;
        state->adsr.Attack(A, D, S, R);
        state->setActive(true);
        graph_phase = 0;
    }else{
        state->adsr.Release([state](bool active) {
            state->setActive(active);
        });
    }
}

void Sine::IARender(SineState * state, double beat, float * lsample, float * rsample)
{

    state->adsr.ARender(beat, lsample, rsample);

    float sample = sinf(state->phase) * state->volume * level * state->adsr;

    state->phase += getPhaseIncrement(state->note + 12*ratio, fine);

    *lsample += sample;
    *rsample += sample;
}