//
// Created by Admin on 09.08.2019.
//

#ifndef PD_SINE_H
#define PD_SINE_H

#include "Instruments/Instrument.h"
#include "ADSR.h"
#include <GUI/Encoder.h>
#include <GUI/Graph.h>
#include <random>

class SineState : public InstrumentState{
public:
    double beat;
    unsigned char velocity;

    ADSR adsr;

    float volume;
    double phase;
    double frequency;
    double phase_increment;
};

class Oscillator : public Instrument<SineState>{

    const int waveform_vis_size = 200;

    std::random_device dev;
    std::mt19937 * rng;
    std::uniform_real_distribution<float> * dist;

    void draw_waveform();

public:

    Encoder * coarse;
    Encoder * fine;
    Encoder * level;
    Encoder * A;
    Encoder * D;
    Encoder * S;
    Encoder * R;

    TimeGraph * graph;

    Text * name;

    int graph_phase;

    int type = 0;

    float k;

    float waveform = 0;
    float old_waveform = 0;

    double phase_shift;
    float ratio;

    const float textheight = 0.3;

    Oscillator(const wchar_t * name, unsigned int num_voices);

    inline float osc(float phase) {
        float s = sinf(phase), w = 0;
        switch (type){
            case 0:
                w = (*dist)(*rng);
                break;
            case 1:
                if (phase / M_PI < 1) w = (phase / M_PI - 1)*2 + 1;
                else w = (phase / M_PI - 1)*(-2) + 1;
                break;
            case 2:
                if (phase / M_PI / 2 < 0.5) w = 1;
                else w = -1;
                break;
            case 3:
                w = phase / M_PI - 1;
                break;
            default:
                break;
        }

        return s * (1 - waveform) + waveform * w;
    }

    GObject * GFindFocusObject(const ndk_helper::Vec2& point) override
    {
        if (visible && globalPosition.contains(point)){
            if (graph->globalPosition.contains(point)) return this;
            return Instrument::GFindFocusObject(point);
        }
        return nullptr;
    }

    GObject * GTapEnd(const ndk_helper::Vec2& v) override ;

    GObject * GDragHandler(const ndk_helper::Vec2& v) override ;
    GObject * GDragBegin(const ndk_helper::Vec2& v) override ;

    void IUpdateState(SineState * state, MData md) override;

    void IARender(SineState * state, double beat, float * lsample, float * rsample) override ;
};


#endif //PD_SINE_H
