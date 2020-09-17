//
// Created by ferluht on 28/04/2020.
//

#include "SplineSynth.h"


SplineSynth::SplineSynth(unsigned int num_voices) : Instrument<SplineSynthState>(num_voices, "SplineSynth"){

    GSetRatio(1.7);

    x = new float[spline_size + 1];
    a = new float[spline_size + 1];
    h = new float[spline_size];
    AA = new float[spline_size];
    l = new float[spline_size + 1];
    u = new float[spline_size + 1];
    z = new float[spline_size + 1];
    c = new float[spline_size + 1];
    b = new float[spline_size];
    d = new float[spline_size];

    rng = new std::mt19937(dev());
    dist = new std::uniform_real_distribution<float>(-1, 1);
    phase_mod = &zero;
    amp_mod = &zero;

    coarse = new GUI::AnalogEncoder("coarse", 1, 0, 10);
    coarse->GPlace({0, 0});
    coarse->GSetHeight(0.2);
    GAttach(coarse);
    MConnect(coarse);

    fine = new GUI::AnalogEncoder("fine", 0, 0, 1);
    fine->GPlace({0, 0.2});
    fine->GSetHeight(0.2);
    GAttach(fine);
    MConnect(fine);

    level = new GUI::AnalogEncoder("level", 0, 0, 1);
    level->GPlace({0, 0.4});
    level->GSetHeight(0.2);
    GAttach(level);
    MConnect(level);

    Y1 = new GUI::AnalogEncoder("y1", 0.1);
    Y1->GPlace({0.1, 0});
    Y1->GSetHeight(0.2);
    GAttach(Y1);
    MConnect(Y1);

    Y2 = new GUI::AnalogEncoder("y2", 0.5);
    Y2->GPlace({0.2, 0});
    Y2->GSetHeight(0.2);
    GAttach(Y2);
    MConnect(Y2);

    Y3 = new GUI::AnalogEncoder("y3", -0.7);
    Y3->GPlace({0.3, 0});
    Y3->GSetHeight(0.2);
    GAttach(Y3);
    MConnect(Y3);

    Y4 = new GUI::AnalogEncoder("y4", 0.2);
    Y4->GPlace({0.4, 0});
    Y4->GSetHeight(0.2);
    GAttach(Y4);
    MConnect(Y4);

//    X1 = new GUI::AnalogEncoder("x1", 0.1, 0.1, 0.9);
//    X1->GPlace({0.1, 0.2});
//    X1->GSetHeight(0.2);
//    GAttach(X1);
//    MConnect(X1);

    X2 = new GUI::AnalogEncoder("x2", 0.2, 0.1, 0.9);
    X2->GPlace({0.2, 0.2});
    X2->GSetHeight(0.2);
    GAttach(X2);
    MConnect(X2);

    X3 = new GUI::AnalogEncoder("x3", 0.3, 0.1, 0.9);
    X3->GPlace({0.3, 0.2});
    X3->GSetHeight(0.2);
    GAttach(X3);
    MConnect(X3);

    X4 = new GUI::AnalogEncoder("x4", 0.5, 0.1, 0.9);
    X4->GPlace({0.4, 0.2});
    X4->GSetHeight(0.2);
    GAttach(X4);
    MConnect(X4);

    M[0] = new GUI::AnalogEncoder("m1", 0, 0, 1);
    M[0]->GPlace({0.1, 0.4});
    M[0]->GSetHeight(0.2);
    GAttach(M[0]);
    MConnect(M[0]);

    M[1] = new GUI::AnalogEncoder("m2", 0, 0, 1);
    M[1]->GPlace({0.2, 0.4});
    M[1]->GSetHeight(0.2);
    GAttach(M[1]);
    MConnect(M[1]);

    M[2] = new GUI::AnalogEncoder("m3", 0, 0, 1);
    M[2]->GPlace({0.3, 0.4});
    M[2]->GSetHeight(0.2);
    GAttach(M[2]);
    MConnect(M[2]);

    M[3] = new GUI::AnalogEncoder("m4", 0, 0, 1);
    M[3]->GPlace({0.4, 0.4});
    M[3]->GSetHeight(0.2);
    GAttach(M[3]);
    MConnect(M[3]);

    A = new GUI::AnalogEncoder("attack", 0.2, 0, 1);
    A->GPlace({0, 0.8});
    A->GSetHeight(0.2);
    GAttach(A);
    MConnect(A);

    D = new GUI::AnalogEncoder("decay", 0.2, 0, 1);
    D->GPlace({0.1, 0.8});
    D->GSetHeight(0.2);
    GAttach(D);
    MConnect(D);

    S = new GUI::AnalogEncoder("sustain", 0.7, 0, 1);
    S->GPlace({0.2, 0.8});
    S->GSetHeight(0.2);
    GAttach(S);
    MConnect(S);

    R = new GUI::AnalogEncoder("release", 1, 0, 5);
    R->GPlace({0.3, 0.8});
    R->GSetHeight(0.2);
    GAttach(R);
    MConnect(R);

    jack = new GUI::Jack(GUI::Jack::OUTPUT);
    jack->GPlace({0.43, 0.865});
    jack->GSetHeight(0.07);
    GAttach(jack);

    tr_coarse = new GUI::AnalogEncoder("coarse", 1, 0, 10);
    tr_coarse->GPlace({0.6, 0.8});
    tr_coarse->GSetHeight(0.2);
    GAttach(tr_coarse);
    MConnect(tr_coarse);

    tr_fine = new GUI::AnalogEncoder("fine", 0, 0, 1);
    tr_fine->GPlace({0.7, 0.8});
    tr_fine->GSetHeight(0.2);
    GAttach(tr_fine);
    MConnect(tr_fine);

    tr_level = new GUI::AnalogEncoder("level", 0, 0, 1);
    tr_level->GPlace({0.8, 0.8});
    tr_level->GSetHeight(0.2);
    GAttach(tr_level);
    MConnect(tr_level);

    tr_jack = new GUI::Jack(GUI::Jack::OUTPUT);
    tr_jack->GPlace({0.93, 0.865});
    tr_jack->GSetHeight(0.07);
    GAttach(tr_jack);

    tr1_coarse = new GUI::AnalogEncoder("coarse", 1, 0, 10);
    tr1_coarse->GPlace({0.6, 0.6});
    tr1_coarse->GSetHeight(0.2);
    GAttach(tr1_coarse);
    MConnect(tr1_coarse);

    tr1_fine = new GUI::AnalogEncoder("fine", 0, 0, 1);
    tr1_fine->GPlace({0.7, 0.6});
    tr1_fine->GSetHeight(0.2);
    GAttach(tr1_fine);
    MConnect(tr1_fine);

    tr1_level = new GUI::AnalogEncoder("level", 0, 0, 1);
    tr1_level->GPlace({0.8, 0.6});
    tr1_level->GSetHeight(0.2);
    GAttach(tr1_level);
    MConnect(tr1_level);

    tr1_jack = new GUI::Jack(GUI::Jack::OUTPUT);
    tr1_jack->GPlace({0.93, 0.665});
    tr1_jack->GSetHeight(0.07);
    GAttach(tr1_jack);

    plot = new GUI::Plot<GUI::TimeGraph>(waveform_vis_size);
    plot->GPlace({0.525, 0.025});
    plot->GSetHeight(0.45);
    plot->GSetWidth(0.45);
    GAttach(plot);

    draw_waveform();

    plot->GSetDragBeginCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
        drag_from = v;
        old_waveform = waveform;
        return plot;
    });

    plot->GSetDragHandlerCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
        float wf = old_waveform + (v.y - drag_from.y)/100;
        if (wf > 1) wf = 1;
        if (wf < 0) wf = 0;
        waveform = wf;
        draw_waveform();
        return plot;
    });

    plot->GSetTapEndCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
        type ++;
        if (type > 3) type = 0;
        draw_waveform();
        return nullptr;
    });
}

void SplineSynth::draw_waveform() {
    for (int i = 0; i < waveform_vis_size; i++){
        float x_ = i*(M_PI*2/waveform_vis_size) - M_PI;
        if (x_ < -M_PI) x_ = -M_PI;
        if (x_ > M_PI) x_ = M_PI;
        plot->graph->update(evaluate_spline(x_) * 0.4);
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

void SplineSynth::IARenderOnce(double beat) {
    x[0]=-M_PI;
    x[1]=x[0] + *X2 * (M_PI - x[0]);
    x[2]=x[1] + *X3 * (M_PI - x[1]);
    x[3]=x[2] + *X4 * (M_PI - x[2]);
    x[4]=M_PI;
    a[0]=*Y1; a[1]=*Y2; a[2]=*Y3; a[3]=*Y4; a[4]=*Y1;

    build_spline();
}

void SplineSynth::IARender(SplineSynthState * state, double beat, float * lsample, float * rsample)
{
    if (state->phase > M_PI) state->phase -= 2*M_PI;
    if (state->tr_phase > M_PI) state->tr_phase -= 2*M_PI;
    if (state->tr1_phase > M_PI) state->tr1_phase -= 2*M_PI;

    output = osc(state->phase + *phase_mod * abs(state->phase)) * state->volume * *level * (1 + *amp_mod);

    state->phase += getPhaseIncrement(state->note + 12*((unsigned char)*coarse) + *fine);
    state->tr_phase += getPhaseIncrement(state->note + 12*(*tr_coarse) + *tr_fine);
    state->tr1_phase += getPhaseIncrement(state->note + 12*(*tr1_coarse) + *tr1_fine);

    float w;
    if (state->tr_phase < 0) w = (state->tr_phase / M_PI)*2 + 1;
    else w = (state->tr_phase / M_PI)*(-2) + 1;
    *tr_jack = w * *tr_level;
//    output += *tr_jack;

    if (state->tr1_phase < 0) w = (state->tr1_phase / M_PI)*2 + 1;
    else w = (state->tr1_phase / M_PI)*(-2) + 1;
    *tr1_jack = w * *tr1_level;
    output += *tr1_jack;

    state->adsr.A = (*A) * (*A);
    state->adsr.D = (*D) * (*D);
    state->adsr.S = (*S) * (*S);
    state->adsr.R = (*R) * (*R);

    float amp = 1;
    if (!state->adsr.ARender(beat, &output, &output)) state->setActive(false);
//    output *= amp;
//    *jack = amp;



    *lsample += output;
    *rsample += output;
}