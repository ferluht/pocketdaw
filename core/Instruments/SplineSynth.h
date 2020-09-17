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
    double tr_phase;
    double tr1_phase;
    double frequency;
    double phase_increment;

    SplineSynthState () {
//        note = 20;
//        volume = 0.5f;
//        setActive(true);
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

    int spline_size = 4;
    float * x, * a, * h, * AA, * l, * u, * z, * c, * b, * d;

    inline void build_spline(){

        /** Step 1 */
        for (int i = 0; i <= spline_size - 1; ++i) h[i] = x[i + 1] - x[i];

        /** Step 2 */
        for (int i = 1; i <= spline_size - 1; ++i)
            AA[i] = 3 * (a[i + 1] - a[i]) / h[i] - 3 * (a[i] - a[i - 1]) / h[i - 1];

        /** Step 3 */
        l[0] = 1;
        u[0] = 0;
        z[0] = 0;

        /** Step 4 */
        for (int i = 1; i <= spline_size - 1; ++i) {
            l[i] = 2 * (x[i + 1] - x[i - 1]) - h[i - 1] * u[i - 1];
            u[i] = h[i] / l[i];
            z[i] = (AA[i] - h[i - 1] * z[i - 1]) / l[i];
        }

        /** Step 5 */
        l[spline_size] = l[0];
        z[spline_size] = z[0];
        c[spline_size] = 0;

        /** Step 6 */
        for (int j = spline_size - 1; j >= 0; --j) {
            c[j] = z[j] - u[j] * c[j + 1];
            b[j] = (a[j + 1] - a[j]) / h[j] - h[j] * (c[j + 1] + 2 * c[j]) / 3;
            d[j] = (c[j + 1] - c[j]) / (3 * h[j]);
        }
    }

    inline float evaluate_spline(float x_) {
        int i = 0;
        while (x_ >= x[i]) i++;
        x_ = x_ - x[i-1];
        float s = d[i-1]*x_*x_*x_ + c[i-1]*x_*x_ + b[i-1]*x_ + a[i-1];

        float phase = (x_ / (x[i] - x[i-1]) - 0.5f) * 2;
        float tr;

        int m_n = i;
        if(m_n > 4) m_n = 4;
        float prop_mix = *M[m_n-1]/0.5f;
        float prop_sharp = 1;
        if (prop_mix > 1) {
            prop_sharp = 2 - prop_mix;
            prop_mix = 1;
        }

        if (phase < 0) tr = a[i-1] + phase + 1 + prop_sharp * (a[i] - a[i-1]) * (phase + 1) / 2;
        else tr = a[i] + 1 - phase - prop_sharp * (a[i] - a[i-1]) * (1 - phase) / 2;

        return s * (1 - prop_mix) + (prop_mix * tr);
    }

    inline double osc(float phase) {
        phase = phase < -M_PI ? M_PI + fmod(phase + M_PI, 2*M_PI) : -M_PI + fmod(phase + M_PI, 2*M_PI);

        float s = evaluate_spline(phase);

        return s;
    }

    float waveform = 0;
    float old_waveform = 0;

    float output = 0;

public:

    GUI::AnalogEncoder * coarse;
    GUI::AnalogEncoder * fine;
    GUI::AnalogEncoder * level;
    GUI::AnalogEncoder * Y1;
    GUI::AnalogEncoder * Y2;
    GUI::AnalogEncoder * Y3;
    GUI::AnalogEncoder * Y4;
//    GUI::AnalogEncoder * X1;
    GUI::AnalogEncoder * X2;
    GUI::AnalogEncoder * X3;
    GUI::AnalogEncoder * X4;
    GUI::AnalogEncoder * M[4];
    GUI::AnalogEncoder * A;
    GUI::AnalogEncoder * D;
    GUI::AnalogEncoder * S;
    GUI::AnalogEncoder * R;

    GUI::AnalogEncoder * tr_coarse;
    GUI::AnalogEncoder * tr_fine;
    GUI::AnalogEncoder * tr_level;
    GUI::Jack * tr_jack;

    GUI::AnalogEncoder * tr1_coarse;
    GUI::AnalogEncoder * tr1_fine;
    GUI::AnalogEncoder * tr1_level;
    GUI::Jack * tr1_jack;


    GUI::Jack * jack;

    GUI::Plot<GUI::TimeGraph> * plot;

    vecmath::Vec2 drag_from;

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
        Y1->MRender(beat);
        Y2->MRender(beat);
        Y3->MRender(beat);
        Y4->MRender(beat);
//        X1->MRender(beat);
        X2->MRender(beat);
        X3->MRender(beat);
        X4->MRender(beat);

        M[0]->MRender(beat);
        M[1]->MRender(beat);
        M[2]->MRender(beat);
        M[3]->MRender(beat);

        A->MRender(beat);
        D->MRender(beat);
        S->MRender(beat);
        R->MRender(beat);

        tr_coarse->MRender(beat);
        tr_fine->MRender(beat);
        tr_level->MRender(beat);
        tr_jack->MRender(beat);

        tr1_coarse->MRender(beat);
        tr1_fine->MRender(beat);
        tr1_level->MRender(beat);
        tr1_jack->MRender(beat);

        jack->MRender(beat);
    }

    void IUpdateState(SplineSynthState * state, MData md) override;

    void IARender(SplineSynthState * state, double beat, float * lsample, float * rsample) override ;

    void IARenderOnce(double beat) override;

    void GDraw(NVGcontext * nvg) override {
        draw_waveform();
        Instrument<SplineSynthState>::GDraw(nvg);

//        float px[4], py[4];
//        px[0] = plot->global.c.x + plot->global.s.x * (x[1] + M_PI) / 2 / M_PI;
//        py[0] = plot->global.c.y + plot->global.s.y * (a[1] + 1) / 2;
//
//        nvgBeginPath(nvg);
//        nvgCircle(nvg, px[0], py[0], 5);
//        nvgStrokeColor(nvg, RED);
//        nvgStrokeWidth(nvg, 2);
//        nvgStroke(nvg);
//        nvgClosePath(nvg);
    }
};



#endif //PD_SPLINESYNTH_H
