//
// Created by ferluht on 06/11/2020.
//

#ifndef ANDROID_APPARATUS_H
#define ANDROID_APPARATUS_H

#include <random>
#include <Instruments/Envelopes/ADSR.h>
#include <GUI/Graph.h>
#include <GUI/Plot.h>
#include <GUI/Led.h>
#include <AudioEffects/Filters/MoogRKSimulationModel.h>
#include "Instrument.h"
#include "../GUI/Encoder.h"

class ApparatusState : public InstrumentState {

public:

    double o1_phase, o2_phase, lfo_phase;
    float note;
    float note_target;
    float note_inc;

    ADSR adsr;

    ApparatusState () {
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

class Apparatus : public Instrument<ApparatusState> {

    const int waveform_vis_size = 200;

    GUI::AnalogEncoder * tune;
    GUI::AnalogEncoder * detune;
    GUI::AnalogEncoder * mix;
    GUI::AnalogEncoder * waveform_o1;
    GUI::AnalogEncoder * waveform_o2;
    GUI::AnalogEncoder * octave_o1;
    GUI::AnalogEncoder * octave_o2;
    GUI::AnalogEncoder * lfo_rate;
    GUI::AnalogEncoder * lfo_fmod;
    GUI::AnalogEncoder * lfo_amod;
    GUI::AnalogEncoder * glide_time;
    GUI::AnalogEncoder * master_amp;
    GUI::AnalogEncoder * A;
    GUI::AnalogEncoder * D;
    GUI::AnalogEncoder * S;
    GUI::AnalogEncoder * R;
    GUI::AnalogEncoder * f_cutoff;
    GUI::AnalogEncoder * f_resonance;
    GUI::AnalogEncoder * f_drive;
    GUI::AnalogEncoder * f_envelope_amt;

    std::random_device dev;
    std::mt19937 * rng;
    std::uniform_real_distribution<float> * dist;

    GUI::Plot<GUI::TimeGraph> * o1_wave;
    GUI::Plot<GUI::TimeGraph> * o2_wave;
    GUI::Plot<GUI::TimeGraph> * envelope;

    RKSimulationMoog * filter;

    vecmath::Vec2 drag_from;
    float old_waveform;

    float lfo_phase = 0;
    float lfo_value = 0;

    inline double osc(double phase, GUI::AnalogEncoder * waveform) {
        phase = phase < -M_PI ? M_PI + fmod(phase + M_PI, 2*M_PI) : -M_PI + fmod(phase + M_PI, 2*M_PI);
        float s = -sin(phase), w = -phase / M_PI;
        return s * (1 - *waveform) + *waveform * w;
    }

    void update_plots(){
        for (int i = 0; i < waveform_vis_size; i++){
            o1_wave->update(osc(i*(M_PI*2/waveform_vis_size) - M_PI, waveform_o1) * 0.9);
            o2_wave->update(osc(i*(M_PI*2/waveform_vis_size) - M_PI, waveform_o2) * 0.9);
            envelope->update(0);
        }
    }

    void update_lfo(){
        lfo_phase += 2 * M_PI * (*lfo_rate / 44100.f);
        if (lfo_phase > (2 * M_PI)) lfo_phase -= 2 * M_PI;
        lfo_value = (sin(lfo_phase) + 1) / 2.f;
    }

public:

    Apparatus() : Instrument<ApparatusState> (1, "Apparatus") {
        GSetRatio(0.9);

        tune = new GUI::AnalogEncoder("tune", 2.5, 0, 10);
        tune->GPlace({0.01,0.01});
        tune->GSetHeight(0.2);
        GAttach(tune);

        detune = new GUI::AnalogEncoder("detune", 0, 0, 1);
        detune->GPlace({0.206,0.01});
        detune->GSetHeight(0.2);
        GAttach(detune);

        mix = new GUI::AnalogEncoder("mix", 0.5, 0, 1);
        mix->GPlace({0.402,0.01});
        mix->GSetHeight(0.2);
        GAttach(mix);

        o1_wave = new GUI::Plot<GUI::TimeGraph>(waveform_vis_size);
        o1_wave->GPlace({0.603, 0.02});
        o1_wave->GSetHeight(0.09);
        o1_wave->GSetWidth(0.18);
        GAttach(o1_wave);

        o2_wave = new GUI::Plot<GUI::TimeGraph>(waveform_vis_size);
        o2_wave->GPlace({0.799, 0.02});
        o2_wave->GSetHeight(0.09);
        o2_wave->GSetWidth(0.18);
        GAttach(o2_wave);

        waveform_o1 = new GUI::AnalogEncoder("waveform", 0, 0, 1);
        waveform_o1->GPlace({0.598, 0.115});
        waveform_o1->GSetHeight(0.09);
        GAttach(waveform_o1);

        waveform_o2 = new GUI::AnalogEncoder("waveform", 0, 0, 1);
        waveform_o2->GPlace({0.794, 0.115});
        waveform_o2->GSetHeight(0.09);
        GAttach(waveform_o2);

        octave_o1 = new GUI::AnalogEncoder("octave", 1, 0, 4);
        octave_o1->setdiscrete(true);
        octave_o1->GPlace({0.696, 0.115});
        octave_o1->GSetHeight(0.09);
        GAttach(octave_o1);

        octave_o2 = new GUI::AnalogEncoder("octave", 1, 0, 4);
        octave_o2->setdiscrete(true);
        octave_o2->GPlace({0.892, 0.115});
        octave_o2->GSetHeight(0.09);
        GAttach(octave_o2);

        A = new GUI::AnalogEncoder("attack", 0.2, 0, 1);
        A->GPlace({0.01, 0.22});
        A->GSetHeight(0.2);
        GAttach(A);
        MConnect(A);

        D = new GUI::AnalogEncoder("decay", 0.2, 0, 1);
        D->GPlace({0.206, 0.22});
        D->GSetHeight(0.2);
        GAttach(D);
        MConnect(D);

        S = new GUI::AnalogEncoder("sustain", 0.7, 0, 1);
        S->GPlace({0.402, 0.22});
        S->GSetHeight(0.2);
        GAttach(S);
        MConnect(S);

        R = new GUI::AnalogEncoder("release", 1, 0, 5);
        R->GPlace({0.598, 0.22});
        R->GSetHeight(0.2);
        GAttach(R);
        MConnect(R);

        envelope = new GUI::Plot<GUI::TimeGraph>(waveform_vis_size);
        envelope->GPlace({0.799, 0.22});
        envelope->GSetHeight(0.19);
        envelope->GSetWidth(0.18);
        GAttach(envelope);

        lfo_rate = new GUI::AnalogEncoder("LFO rate", 2, 0.01, 20);
        lfo_rate->GPlace({0.01, 0.43});
        lfo_rate->GSetHeight(0.2);
        GAttach(lfo_rate);
        MConnect(lfo_rate);

        lfo_fmod = new GUI::AnalogEncoder("F mod", 0.2, 0, 1);
        lfo_fmod->GPlace({0.206, 0.43});
        lfo_fmod->GSetHeight(0.2);
        GAttach(lfo_fmod);
        MConnect(lfo_fmod);

        lfo_amod = new GUI::AnalogEncoder("A mod", 0.7, 0, 1);
        lfo_amod->GPlace({0.402, 0.43});
        lfo_amod->GSetHeight(0.2);
        GAttach(lfo_amod);
        MConnect(lfo_amod);

        glide_time = new GUI::AnalogEncoder("glide", 0, 0, 1);
        glide_time->GPlace({0.598, 0.43});
        glide_time->GSetHeight(0.2);
        GAttach(glide_time);

        master_amp = new GUI::AnalogEncoder("AMP", 0, 0, 1);
        master_amp->GPlace({0.799, 0.43});
        master_amp->GSetHeight(0.2);
        GAttach(master_amp);

        rng = new std::mt19937(dev());
        dist = new std::uniform_real_distribution<float>(-1, 1);

        update_plots();

        f_cutoff = new GUI::AnalogEncoder("cutoff", 1000, 0, 15000);
        f_cutoff->GPlace({0.01, 0.64});
        f_cutoff->GSetHeight(0.2);
        GAttach(f_cutoff);
        MConnect(f_cutoff);

        f_resonance = new GUI::AnalogEncoder("resonance", 0, 0, 10);
        f_resonance->GPlace({0.206, 0.64});
        f_resonance->GSetHeight(0.2);
        GAttach(f_resonance);
        MConnect(f_resonance);

        f_envelope_amt = new GUI::AnalogEncoder("envelope", 0, -5, 5);
        f_envelope_amt->GPlace({0.402, 0.64});
        f_envelope_amt->GSetHeight(0.2);
        GAttach(f_envelope_amt);
        MConnect(f_envelope_amt);

        filter = new RKSimulationMoog(sample_rate);
    }

    void IUpdateState(ApparatusState *state, MData md) {
        if (((md.status & 0xF0) == NOTEON_HEADER) && (md.data2 != 0)) {
            if (*glide_time > 1.0 / sample_rate) {
                state->note_inc = ((md.data1 - 24) - state->note) / (*glide_time * sample_rate);
            } else {
                state->note_inc = 0;
                state->note = md.data1 - 24;
            }
            state->note_target = md.data1 - 24;
            state->adsr.attack(md.beat);
        }else{
            state->adsr.release(md.beat);
        }
    }

    void MRender(double beat) override {
        tune->MRender(beat);
        detune->MRender(beat);
        octave_o2->MRender(beat);
        octave_o1->MRender(beat);
        mix->MRender(beat);
        waveform_o1->MRender(beat);
        waveform_o2->MRender(beat);
        master_amp->MRender(beat);
        A->MRender(beat);
        D->MRender(beat);
        S->MRender(beat);
        R->MRender(beat);
        f_cutoff->MRender(beat);
        f_resonance->MRender(beat);
    }

    void IARender(ApparatusState * state, double beat, float * lsample, float * rsample) override {
        float first = osc(state->o1_phase, waveform_o1);
        float second = osc(state->o2_phase, waveform_o2);

        state->adsr.A = (*A) * (*A);
        state->adsr.D = (*D) * (*D);
        state->adsr.S = (*S) * (*S);
        state->adsr.R = (*R) * (*R);
        float amp = 1;
        state->adsr.ARender(beat, &amp, &amp);// state->setActive(false);

        update_lfo();

        float sample = (first * *mix + second * (1 - *mix)) * *master_amp * (1 - lfo_value * *lfo_amod) * amp;

        float fmod = amp * *f_envelope_amt * *f_cutoff;
        if (fmod < 1) fmod = 1;
        if (fmod > 15000) fmod = 15000;
        filter->SetCutoff(*f_cutoff + fmod);
        filter->SetResonance(*f_resonance);
        filter->Process(&sample, 1);

//        state->adsr.A = (*A) * (*A);
//        state->adsr.D = (*D) * (*D);
//        state->adsr.S = (*S) * (*S);
//        state->adsr.R = (*R) * (*R);
//        if(!state->adsr.ARender(beat, &sample, &sample)) state->setActive(false);

        state->o1_phase += getPhaseIncrement((*octave_o1 + *tune) * 12 + *detune + state->note);
        if (state->o1_phase > 6.283f) state->o1_phase -= 6.283f;

        state->o2_phase += getPhaseIncrement((*octave_o2 + *tune) * 12 - *detune + state->note);
        if (state->o2_phase > 6.283f) state->o2_phase -= 6.283f;

        if (abs(state->note - state->note_target) > 0.01) {
            state->note += state->note_inc;
        }

        *lsample += sample;
        *rsample += sample;
    }

    virtual void GDraw(NVGcontext * nvg) override {
        update_plots();
        Instrument<ApparatusState>::GDraw(nvg);
    }

};


#endif //ANDROID_APPARATUS_H
