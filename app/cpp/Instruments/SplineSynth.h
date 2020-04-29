//
// Created by ferluht on 28/04/2020.
//

#ifndef PD_SPLINESYNTH_H
#define PD_SPLINESYNTH_H

#include <random>
#include <Instruments/Envelopes/ADSR.h>
#include <GUI/Graph.h>
#include <GUI/Plot.h>
#include <GUI/Led.h>
#include "Instrument.h"
#include "../GUI/Encoder.h"
#include <Utils/Spline.h>

class SplineSynthState : public InstrumentState{
public:
    double beat;
    unsigned char velocity;
    float note;

    ADSR adsr;

    float volume;
    double phase;
    double frequency;
    double phase_increment;

    SplineSynthState () {
        setActive(true);
    }
};

class SplineSynth : public Instrument<SplineSynthState>{

    const int waveform_vis_size = 200;
    float zero = 0;

    std::random_device dev;
    std::mt19937 * rng;
    std::uniform_real_distribution<float> * dist;

    void draw_waveform();

    float * phase_mod;
    float * amp_mod;

    inline double osc(float phase) {
//        phase = phase < -M_PI ? M_PI + fmod(phase + M_PI, 2*M_PI) : -M_PI + fmod(phase + M_PI, 2*M_PI);
//
//        float x0 = -M_PI;
//        float x1 = -M_PI + 0.2;
//        float x2 = -M_PI/2;
//        float x3 = M_PI/2;
//        float x4 = M_PI;
//
//        float y0 = *R;
//        float y1 = *A;
//        float y2 = *D;
//        float y3 = *S;
//        float y4 = *R;
//
//        float s = y0 * (phase - x1) * (phase - x2) * (phase - x3) * (phase - x4) / (x0 - x1) / (x0 - x2) / (x0 - x3) / (x0 - x4) +
//                y1 * (phase - x0) * (phase - x2) * (phase - x3) * (phase - x4) / (x1 - x0) / (x1 - x2) / (x1 - x3) / (x1 - x4) +
//                y2 * (phase - x0) * (phase - x1) * (phase - x3) * (phase - x4) / (x2 - x0) / (x2 - x1) / (x2 - x3) / (x2 - x4) +
//                y3 * (phase - x0) * (phase - x1) * (phase - x2) * (phase - x4) / (x3 - x0) / (x3 - x1) / (x3 - x2) / (x3 - x4) +
//                y4 * (phase - x0) * (phase - x1) * (phase - x2) * (phase - x3) / (x4 - x0) / (x4 - x1) / (x4 - x2) / (x4 - x3);

        std::vector<double> X(6), Y(6);
        X[0]=-M_PI*2; X[1]=-M_PI; X[2]=-M_PI/2; X[3]=M_PI/2; X[4]=M_PI, X[5]=M_PI*2;
        Y[0]=*S; Y[1]=*A; Y[2]=*D; Y[3]=*S; Y[4]=*A; Y[5]=*D;

        tk::spline s;
        s.set_points(X,Y);
        return s(phase);

//        return s;
    }

    float waveform = 0;
    float old_waveform = 0;

    float output = 0;

public:

    GUI::AnalogEncoder * coarse;
    GUI::AnalogEncoder * fine;
    GUI::AnalogEncoder * level;
    GUI::AnalogEncoder * A;
    GUI::AnalogEncoder * D;
    GUI::AnalogEncoder * S;
    GUI::AnalogEncoder * R;

    GUI::Plot<GUI::TimeGraph> * plot;

    Vec2 drag_from;

    int graph_phase;

    int type = 0;

    float k;

    double phase_shift;

    const float textheight = 0.3;

    SplineSynth(unsigned int num_voices);

    operator float * () { return &output; }

    void MRender(double beat) override {
        coarse->MRender(beat);
        fine->MRender(beat);
        level->MRender(beat);
        A->MRender(beat);
        D->MRender(beat);
        S->MRender(beat);
        R->MRender(beat);
    }

    void IUpdateState(SplineSynthState * state, MData md) override;

    void IARender(SplineSynthState * state, double beat, float * lsample, float * rsample) override ;

    void GDraw(NVGcontext * nvg) override {
        draw_waveform();
        Instrument<SplineSynthState>::GDraw(nvg);
    }
};



#endif //PD_SPLINESYNTH_H
