//
// Created by Admin on 09.08.2019.
//

#include "Oscillator.h"

Oscillator::Oscillator(const char * name_, unsigned int num_voices) : Instrument<SineState>(num_voices, "Osc"){

    shape->setRatio(2);

    rng = new std::mt19937(dev());
    dist = new std::uniform_real_distribution<float>(-1, 1);
    phase_mod = &zero;
    amp_mod = &zero;

    coarse = new GUI::Encoder("coarse", 0, 1, -1, 10);
    coarse->shape->lPlace({0.025, 0.05});
    coarse->shape->lSetHeight(0.4);
    GAttach(coarse);
    MConnect(coarse);

    fine = new GUI::Encoder("fine", 0, 2, 0, 1);
    fine->shape->lPlace({0.225, 0.05});
    fine->shape->lSetHeight(0.4);
    GAttach(fine);
    MConnect(fine);

    level = new GUI::Encoder("level", 0, 3, 0, 1);
    level->shape->lPlace({0.425, 0.05});
    level->shape->lSetHeight(0.4);
    GAttach(level);
    MConnect(level);

    A = new GUI::Encoder("attack", 0, 5, 0, 1);
    A->shape->lPlace({0.625, 0.05});
    A->shape->lSetHeight(0.4);
    GAttach(A);
    MConnect(A);

    D = new GUI::Encoder("decay", 0, 6, 0, 1);
    D->shape->lPlace({0.025, 0.5});
    D->shape->lSetHeight(0.4);
    GAttach(D);
    MConnect(D);

    S = new GUI::Encoder("sustain", 0, 7, 0, 1);
    S->shape->lPlace({0.225, 0.5});
    S->shape->lSetHeight(0.4);
    GAttach(S);
    MConnect(S);

    R = new GUI::Encoder("release", 0, 8, 0, 5);
    R->shape->lPlace({0.425, 0.5});
    R->shape->lSetHeight(0.4);
    GAttach(R);
    MConnect(R);

//    name = new Text("Fonts/Roboto-Regular.ttf", name_);
//    name->place(0.80, 0.05);
//    name->setHeight(0.4);
//    GAttach(name);

    *A = 0.2;
    *D = 1.2;
    *S = 0.7;
    *R = 0.5;

//    graph = new TimePlot(waveform_vis_size);
//    graph->place(0.625, 0.5);
//    graph->setHeight(0.4);
//    graph->setWidth(0.35);
//    GAttach(graph);

    draw_waveform();
}

void Oscillator::draw_waveform() {
    for (int i = 0; i < waveform_vis_size; i++){
//        graph->update(osc(i*(M_PI*2/waveform_vis_size) - M_PI) * 0.9);
    }
}

void Oscillator::IUpdateState(SineState *state, MData md){
    if (((md.status & 0xF0) == NOTEON_HEADER) && (md.data2 != 0)) {
        state->note = md.data1;
        state->volume = (float)md.data2/127.f;
        if (!state->isActive()) state->phase = -2*M_PI;
        state->adsr.attack(md.beat);
        state->setActive(true);
        graph_phase = 0;
    }else{
        state->adsr.release(md.beat);
    }
}

GUI::GObject * Oscillator::GTapEnd(const ndk_helper::Vec2& v)
{
//    if (graph->globalPosition.contains(v)){
//        type ++;
//        if (type > 3) type = 0;
//        draw_waveform();
//    }
    return nullptr;
}

GUI::GObject * Oscillator::GDragHandler(const ndk_helper::Vec2 &v) {

//    if (graph->globalPosition.contains(drag_from)) {
//        float wf = old_waveform + (v.y_ - drag_from.y_)/100;
//        if (wf > 1) wf = 1;
//        if (wf < 0) wf = 0;
//        waveform = wf;
//        draw_waveform();
//    }

    return this;
}

GUI::GObject * Oscillator::GDragBegin(const ndk_helper::Vec2 &v) {
    drag_from = v;
    old_waveform = waveform;
    return this;
}

void Oscillator::IARender(SineState * state, double beat, float * lsample, float * rsample)
{
    if (state->phase > M_PI) state->phase -= 2*M_PI;

    output = osc(state->phase + *phase_mod * abs(state->phase)) * state->volume * *level * (1 + *amp_mod);

    state->adsr.A = (*A) * (*A);
    state->adsr.D = (*D) * (*D);
    state->adsr.S = (*S) * (*S);
    state->adsr.R = (*R) * (*R);
    if (!state->adsr.ARender(beat, &output, &output)) state->setActive(false);

    state->phase += getPhaseIncrement(state->note + 12*((unsigned char)*coarse) + *fine);

    *lsample += output;
    *rsample += output;
}

void Oscillator::OSetPhaseModulator(float *mod_) {
    phase_mod = mod_;
}

void Oscillator::OResetPhaseModulator(){
    phase_mod = &zero;
}

void Oscillator::OSetAmpModulator(float *mod_){
    amp_mod = mod_;
}

void Oscillator::OResetAmpModulator(){
    amp_mod = &zero;
}

