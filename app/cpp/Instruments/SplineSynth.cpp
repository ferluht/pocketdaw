//
// Created by ferluht on 28/04/2020.
//

#include "SplineSynth.h"


SplineSynth::SplineSynth(unsigned int num_voices) : Instrument<SplineSynthState>(num_voices, "SplineSynth"){

    GSetRatio(2);

    rng = new std::mt19937(dev());
    dist = new std::uniform_real_distribution<float>(-1, 1);
    phase_mod = &zero;
    amp_mod = &zero;

    coarse = new GUI::AnalogEncoder("coarse", 0, -1, 10);
    coarse->GPlace({0.025, 0.05});
    coarse->GSetHeight(0.4);
    GAttach(coarse);
    MConnect(coarse);

    fine = new GUI::AnalogEncoder("fine", 0, 0, 1);
    fine->GPlace({0.225, 0.05});
    fine->GSetHeight(0.4);
    GAttach(fine);
    MConnect(fine);

    level = new GUI::AnalogEncoder("level", 0, 0, 1);
    level->GPlace({0.425, 0.05});
    level->GSetHeight(0.4);
    GAttach(level);
    MConnect(level);

    A = new GUI::AnalogEncoder("y1", 0, -1, 1);
    A->GPlace({0.625, 0.05});
    A->GSetHeight(0.4);
    GAttach(A);
    MConnect(A);

    D = new GUI::AnalogEncoder("y2", 0, -1, 1);
    D->GPlace({0.025, 0.5});
    D->GSetHeight(0.4);
    GAttach(D);
    MConnect(D);

    S = new GUI::AnalogEncoder("y3", 0, -1, 1);
    S->GPlace({0.225, 0.5});
    S->GSetHeight(0.4);
    GAttach(S);
    MConnect(S);

    R = new GUI::AnalogEncoder("y4", 0, -1, 1);
    R->GPlace({0.425, 0.5});
    R->GSetHeight(0.4);
    GAttach(R);
    MConnect(R);

//    name = new Text("Fonts/Roboto-Regular.ttf", name_);
//    name->place(0.80, 0.05);
//    name->setHeight(0.4);
//    GAttach(name);

//    *A = 0.2;
//    *D = 1.2;
//    *S = 0.7;
//    *R = 0.5;

    plot = new GUI::Plot<GUI::TimeGraph>(waveform_vis_size);
    plot->GPlace({0.625, 0.5});
    plot->GSetHeight(0.4);
    plot->GSetWidth(0.35);
    GAttach(plot);

    draw_waveform();

    plot->GSetDragBeginCallback([this](const Vec2& v) -> GUI::GObject * {
        drag_from = v;
        old_waveform = waveform;
        return plot;
    });

    plot->GSetDragHandlerCallback([this](const Vec2& v) -> GUI::GObject * {
        float wf = old_waveform + (v.y - drag_from.y)/100;
        if (wf > 1) wf = 1;
        if (wf < 0) wf = 0;
        waveform = wf;
        draw_waveform();
        return plot;
    });

    plot->GSetTapEndCallback([this](const Vec2& v) -> GUI::GObject * {
        type ++;
        if (type > 3) type = 0;
        draw_waveform();
        return nullptr;
    });
}

void SplineSynth::draw_waveform() {
    for (int i = 0; i < waveform_vis_size; i++){
        plot->graph->update(osc(i*(M_PI*2/waveform_vis_size) - M_PI) * 0.9);
    }
}

void SplineSynth::IUpdateState(SplineSynthState *state, MData md){
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

void SplineSynth::IARender(SplineSynthState * state, double beat, float * lsample, float * rsample)
{
    if (state->phase > M_PI) state->phase -= 2*M_PI;

    output = osc(state->phase + *phase_mod * abs(state->phase)) * state->volume * *level * (1 + *amp_mod);

    state->phase += getPhaseIncrement(state->note + 12*((unsigned char)*coarse) + *fine);

    *lsample += output;
    *rsample += output;
}