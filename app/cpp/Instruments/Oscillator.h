//
// Created by Admin on 09.08.2019.
//

#ifndef PD_SINE_H
#define PD_SINE_H

#include "Instrument.h"
#include <Instruments/Envelopes/ADSR.h>
#include <GUI/Encoder.h>
#include <random>
#include <GUI/Plot.h>

class SineState : public InstrumentState{
public:
    double beat;
    unsigned char velocity;
    float note;

    ADSR adsr;

    float volume;
    double phase;
    double frequency;
    double phase_increment;
};

class Oscillator : public Instrument<SineState>{

    const int waveform_vis_size = 200;
    float zero = 0;

    std::random_device dev;
    std::mt19937 * rng;
    std::uniform_real_distribution<float> * dist;

    void draw_waveform();

    float * phase_mod;
    float * amp_mod;

    inline double osc(double phase) {
        phase = phase < -M_PI ? M_PI + fmod(phase + M_PI, 2*M_PI) : -M_PI + fmod(phase + M_PI, 2*M_PI);
        float s = -sin(phase), w = 0;
        switch (type){
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

        return s * (1 - waveform) + waveform * w;
    }

    float waveform = 0;
    float old_waveform = 0;

    float output = 0;

public:

    GUI::Encoder * coarse;
    GUI::Encoder * fine;
    GUI::Encoder * level;
    GUI::Encoder * A;
    GUI::Encoder * D;
    GUI::Encoder * S;
    GUI::Encoder * R;

    GUI::Plot<GUI::TimeGraph> * plot;

    Vec2 drag_from;

    int graph_phase;

    int type = 0;

    float k;

    double phase_shift;

    const float textheight = 0.3;

    Oscillator(const char * name, unsigned int num_voices);

    operator float * () { return &output; }

    float operator+(Oscillator &osc2)
    {
        return this->output + osc2.output;
    }

//    GUI::GObject * GFindFocusObject(const ndk_helper::Vec2& point) override
//    {
//        if (visible && globalPosition.contains(point)){
//            if (graph->globalPosition.contains(point)) return this;
//            return Instrument::GFindFocusObject(point);
//        }
//        return nullptr;
//    }

    GUI::GObject * GTapEnd(const ndk_helper::Vec2& v) override ;

    GUI::GObject * GDragHandler(const ndk_helper::Vec2& v) override ;
    GUI::GObject * GDragBegin(const ndk_helper::Vec2& v) override ;

    void IUpdateState(SineState * state, MData md) override;

    void OSetPhaseModulator(float * mod_);
    void OResetPhaseModulator();

    void OSetAmpModulator(float * mod_);
    void OResetAmpModulator();

    void IARender(SineState * state, double beat, float * lsample, float * rsample) override ;
};


#endif //PD_SINE_H
