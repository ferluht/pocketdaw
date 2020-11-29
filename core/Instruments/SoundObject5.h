//
// Created by ferluht on 13/04/2020.
//

#ifndef PD_SOUNDOBJECT5_H
#define PD_SOUNDOBJECT5_H

#include <random>
#include <Instruments/Envelopes/ADSR.h>
#include <GUI/Graph.h>
#include <GUI/Plot.h>
#include <GUI/Led.h>
#include "Instrument.h"
#include "../GUI/Encoder.h"

class SoundObject5State : public InstrumentState {

public:

    double o1_phase, o2_phase, lfo_phase;
    float note;
    float note_target;
    float note_inc;

    ADSR adsr;

    SoundObject5State () {
        o1_phase = 0;
        o2_phase = 0;
        lfo_phase = 0;
        setActive(true);

        adsr.A = 0.2;
        adsr.D = 0.2;
        adsr.S = 0.9;
        adsr.R = 0.3;
    }

};

class SoundObject5 : public Instrument<SoundObject5State> {

    const int waveform_vis_size = 200;

    GUI::AnalogEncoder * o1_coarse;
    GUI::AnalogEncoder * o2_coarse;
    GUI::AnalogEncoder * o1_fine;
    GUI::AnalogEncoder * o2_fine;
    GUI::AnalogEncoder * lfo_rate;
    GUI::AnalogEncoder * o12_xmod;
    GUI::AnalogEncoder * o21_xmod;
    GUI::AnalogEncoder * mix_o1;
    GUI::AnalogEncoder * mix_o2;
    GUI::AnalogEncoder * mix_noise;
    GUI::AnalogEncoder * mix_amp;
    GUI::AnalogEncoder * glide_time;
    GUI::AnalogEncoder * A;
    GUI::AnalogEncoder * D;
    GUI::AnalogEncoder * S;
    GUI::AnalogEncoder * R;

    std::random_device dev;
    std::mt19937 * rng;
    std::uniform_real_distribution<float> * dist;

    GUI::Plot<GUI::TimeGraph> * o1_wave;
    GUI::Plot<GUI::TimeGraph> * o2_wave;

    float o_waveform[2];
    int o_type[2];

    vecmath::Vec2 drag_from;
    float old_waveform;

    GUI::Jack * jack;

    GUI::Led * adsr_led;

    inline double osc(double phase, int osc_num) {
        phase = phase < -M_PI ? M_PI + fmod(phase + M_PI, 2*M_PI) : -M_PI + fmod(phase + M_PI, 2*M_PI);
        float s = -sin(phase), w = 0;
        switch (o_type[osc_num]){
            case 0:
                w = (*dist)(*rng);
                break;
            case 1:
                if (phase < 0) w = (phase / M_PI)*2 + 1;
                else w = (phase / M_PI)*(-2) + 1;
                break;
            case 2:
                if (phase < 0) w = 1;
                else w = -1;
                break;
            case 3:
                w = phase / M_PI;
                break;
            default:
                break;
        }

        return s * (1 - o_waveform[osc_num]) + o_waveform[osc_num] * w;
    }

public:

    SoundObject5() : Instrument<SoundObject5State> (1, "Object5") {
        GSetRatio(0.8);

        o1_coarse = new GUI::AnalogEncoder("coarse", 0, 2.5, 10);
        o1_coarse->GPlace({0.01,0.01});
        o1_coarse->GSetHeight(0.2);
        GAttach(o1_coarse);

        o1_fine = new GUI::AnalogEncoder("fine", 0, -1, 1);
        o1_fine->GPlace({0.23,0.01});
        o1_fine->GSetHeight(0.2);
        GAttach(o1_fine);

        o12_xmod = new GUI::AnalogEncoder("xmod", 0, 0, 1);
        o12_xmod->GPlace({0.45,0.01});
        o12_xmod->GSetHeight(0.2);
        GAttach(o12_xmod);

        o1_wave = new GUI::Plot<GUI::TimeGraph>(waveform_vis_size);
        o1_wave->GPlace({0.68, 0.02});
        o1_wave->GSetHeight(0.19);
        o1_wave->GSetWidth(0.31);
        GAttach(o1_wave);

        o2_coarse = new GUI::AnalogEncoder("coarse", 0, 2.5, 10);
        o2_coarse->GPlace({0.01,0.22});
        o2_coarse->GSetHeight(0.2);
        GAttach(o2_coarse);

        o2_fine = new GUI::AnalogEncoder("fine", 0, -1, 1);
        o2_fine->GPlace({0.23,0.22});
        o2_fine->GSetHeight(0.2);
        GAttach(o2_fine);

        o21_xmod = new GUI::AnalogEncoder("xmod", 0, 0, 1);
        o21_xmod->GPlace({0.45,0.22});
        o21_xmod->GSetHeight(0.2);
        GAttach(o21_xmod);

        o2_wave = new GUI::Plot<GUI::TimeGraph>(waveform_vis_size);
        o2_wave->GPlace({0.68, 0.23});
        o2_wave->GSetHeight(0.19);
        o2_wave->GSetWidth(0.31);
        GAttach(o2_wave);

        mix_o1 = new GUI::AnalogEncoder("o1 mix", 0, 0.5, 1);
        mix_o1->GPlace({0.01,0.43});
        mix_o1->GSetHeight(0.2);
        GAttach(mix_o1);

        mix_o2 = new GUI::AnalogEncoder("o2 mix", 0, 0.5, 1);
        mix_o2->GPlace({0.23,0.43});
        mix_o2->GSetHeight(0.2);
        GAttach(mix_o2);

        mix_noise = new GUI::AnalogEncoder("noise", 0, 0, 1);
        mix_noise->GPlace({0.45,0.43});
        mix_noise->GSetHeight(0.2);
//        GAttach(mix_noise);

        glide_time = new GUI::AnalogEncoder("glide", 0, 0, 1);
        glide_time->GPlace({0.45,0.43});
        glide_time->GSetHeight(0.2);
        GAttach(glide_time);

        mix_amp = new GUI::AnalogEncoder("mix amp", 0, 0, 1);
        mix_amp->GPlace({0.73,0.43});
        mix_amp->GSetHeight(0.2);
        GAttach(mix_amp);

        A = new GUI::AnalogEncoder("attack", 0.2, 0, 1);
        A->GPlace({0.01, 0.79});
        A->GSetHeight(0.2);
        GAttach(A);
        MConnect(A);

        D = new GUI::AnalogEncoder("decay", 0.2, 0, 1);
        D->GPlace({0.23, 0.79});
        D->GSetHeight(0.2);
        GAttach(D);
        MConnect(D);

        S = new GUI::AnalogEncoder("sustain", 0.7, 0, 1);
        S->GPlace({0.45, 0.79});
        S->GSetHeight(0.2);
        GAttach(S);
        MConnect(S);

        R = new GUI::AnalogEncoder("release", 1, 0, 5);
        R->GPlace({0.67, 0.79});
        R->GSetHeight(0.2);
        GAttach(R);
        MConnect(R);

        rng = new std::mt19937(dev());
        dist = new std::uniform_real_distribution<float>(-1, 1);

        o_waveform[0] = 0;
        o_waveform[1] = 0;

        o_type[0] = 0;
        o_type[1] = 0;

        for (int i = 0; i < waveform_vis_size; i++){
            o1_wave->update(osc(i*(M_PI*2/waveform_vis_size) - M_PI, 0) * 0.9);
            o2_wave->update(osc(i*(M_PI*2/waveform_vis_size) - M_PI, 1) * 0.9);
        }

        o1_wave->GSetDragBeginCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
            drag_from = v;
            old_waveform = o_waveform[0];
            return o1_wave;
        });

        o1_wave->GSetDragHandlerCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
            float wf = old_waveform + (v.y - drag_from.y)/100;
            if (wf > 1) wf = 1;
            if (wf < 0) wf = 0;
            o_waveform[0] = wf;
            for (int i = 0; i < waveform_vis_size; i++){
                o1_wave->update(osc(i*(M_PI*2/waveform_vis_size) - M_PI, 0) * 0.9);
            }
            return o1_wave;
        });

        o1_wave->GSetTapEndCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
            o_type[0] ++;
            if (o_type[0] > 3) o_type[0] = 0;
            for (int i = 0; i < waveform_vis_size; i++){
                o1_wave->update(osc(i*(M_PI*2/waveform_vis_size) - M_PI, 0) * 0.9);
            }
            return nullptr;
        });

        o2_wave->GSetDragBeginCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
            drag_from = v;
            old_waveform = o_waveform[1];
            return o2_wave;
        });

        o2_wave->GSetDragHandlerCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
            float wf = old_waveform + (v.y - drag_from.y)/100;
            if (wf > 1) wf = 1;
            if (wf < 0) wf = 0;
            o_waveform[1] = wf;
            for (int i = 0; i < waveform_vis_size; i++){
                o2_wave->update(osc(i*(M_PI*2/waveform_vis_size) - M_PI, 1) * 0.9);
            }
            return o2_wave;
        });

        o2_wave->GSetTapEndCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
            o_type[1] ++;
            if (o_type[1] > 3) o_type[1] = 0;
            for (int i = 0; i < waveform_vis_size; i++){
                o2_wave->update(osc(i*(M_PI*2/waveform_vis_size) - M_PI, 1) * 0.9);
            }
            return nullptr;
        });

        jack = new GUI::Jack(GUI::Jack::OUTPUT);
        jack->GPlace({0.89, 0.80});
        jack->GSetWidth(0.1);
        GAttach(jack);

        adsr_led = new GUI::Led(false);
        adsr_led->GPlace({0.94, 0.96});
        adsr_led->GSetHeight(0.05);
        GAttach(adsr_led);
    }

    void IUpdateState(SoundObject5State *state, MData md) {
        if (((md.status & 0xF0) == NOTEON_HEADER) && (md.data2 != 0)) {
            if (*glide_time > 1.0 / sample_rate) {
                state->note_inc = ((md.data1 - 24) - state->note) / (*glide_time * sample_rate);
            } else {
                state->note_inc = 0;
                state->note = md.data1 - 24;
            }
            state->note_target = md.data1 - 24;
            state->adsr.attack(md.beat);
            adsr_led->toggle();
        }else{
            state->adsr.release(md.beat);
            adsr_led->toggle();
        }
    }

    void MRender(double beat) override {
        o1_coarse->MRender(beat);
        o1_fine->MRender(beat);
        o2_coarse->MRender(beat);
        o2_fine->MRender(beat);
        o12_xmod->MRender(beat);
        o21_xmod->MRender(beat);
        mix_o1->MRender(beat);
        mix_o2->MRender(beat);
        mix_noise->MRender(beat);
        mix_amp->MRender(beat);
        jack->MRender(beat);
        A->MRender(beat);
        D->MRender(beat);
        S->MRender(beat);
        R->MRender(beat);
    }

    void IARender(SoundObject5State * state, double beat, float * lsample, float * rsample) override {
        double first = osc(state->o1_phase, 0);
        double second = osc(state->o2_phase, 1);

        state->adsr.A = (*A) * (*A);
        state->adsr.D = (*D) * (*D);
        state->adsr.S = (*S) * (*S);
        state->adsr.R = (*R) * (*R);
        float amp = 1;
        state->adsr.ARender(beat, &amp, &amp);
        *jack = amp;

        float sample = ( first * *mix_o1 + second * *mix_o2 + (*dist)(*rng) * *mix_noise ) * *mix_amp;

//        state->adsr.A = (*A) * (*A);
//        state->adsr.D = (*D) * (*D);
//        state->adsr.S = (*S) * (*S);
//        state->adsr.R = (*R) * (*R);
//        if(!state->adsr.ARender(beat, &sample, &sample)) state->setActive(false);

        state->o1_phase += getPhaseIncrement(*o1_coarse * 12 + *o1_fine + state->note) * (1 + 5 * *o12_xmod * second);
        if (state->o1_phase > 6.283f) state->o1_phase -= 6.283f;

        state->o2_phase += getPhaseIncrement(*o2_coarse * 12 + *o2_fine + state->note) * (1 + 5 * *o21_xmod * first);
        if (state->o2_phase > 6.283f) state->o2_phase -= 6.283f;

        if (abs(state->note - state->note_target) > 0.01) {
            state->note += state->note_inc;
        }

        *lsample += sample;
        *rsample += sample;
    }

//    struct BSoundobject5 {
//        uint8_t id = 0x02;
//        uint8_t freq1_int = 0;
//        uint8_t freq1_frac = 0;
//        uint8_t freq2_int = 0;
//        uint8_t freq2_frac = 0;
//        uint8_t xmod1_int = 0;
//        uint8_t xmod1_frac = 0;
//        uint8_t xmod2_int = 0;
//        uint8_t xmod2_frac = 0;
//    };
//
//    int BRender (uint8_t * data) override {
//        BSoundobject5 bso5 = {
//                0x02,
//                static_cast<uint8_t>(*o1_coarse),
//                static_cast<uint8_t>((*o1_coarse * 100) % 100),
//                static_cast<uint8_t>(*o2_coarse),
//                static_cast<uint8_t>((*o2_coarse * 100) % 100),
//                static_cast<uint8_t>(*o12_xmod),
//                static_cast<uint8_t>((*o12_xmod * 100) % 100),
//                static_cast<uint8_t>(*o21_xmod),
//                static_cast<uint8_t>((*o21_xmod * 100) % 100)
//        };
//        std::memcpy(data, &bso5, sizeof(BSoundobject5));
//        return sizeof(BSoundobject5);
//    }

};


#endif //PD_SOUNDOBJECT5_H
