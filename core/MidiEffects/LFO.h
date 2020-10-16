//
// Created by ibelikov on 01.01.20.
//

#ifndef PD_LFO_H
#define PD_LFO_H


#include <GUI/Jack.h>
#include <GUI/Encoder.h>
#include <GUI/Graph.h>
#include <GUI/Plot.h>
#include <random>
#include "MidiEffect.h"

class LFO : public MidiEffect {

    GUI::Jack * jack;
    GUI::AnalogEncoder * rate;
    GUI::Plot<GUI::TimeGraph> * plot;

    int counter = 0;

    double phase = 0;
    double last_beat = 0;

    float waveform = 0;
    float old_waveform = 0;

    float value;

    std::random_device dev;
    std::mt19937 * rng;
    std::uniform_real_distribution<float> * dist;

    int type = 0;
    double old_phase = 0;
    float w = 0;

    vecmath::Vec2 drag_from;

    inline double osc(double phase) {
        phase = phase < -M_PI ? M_PI + fmod(phase + M_PI, 2*M_PI) : -M_PI + fmod(phase + M_PI, 2*M_PI);
        float s = -sin(phase);
        switch (type){
            case 0:
                if ((old_phase < 0) && (phase >= 0))
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
        old_phase = phase;
        return s * (1 - waveform) + waveform * w;
    }

public:

    LFO() : MidiEffect("LFO") {
        rng = new std::mt19937(dev());
        dist = new std::uniform_real_distribution<float>(-1, 1);

        GSetRatio(0.3);

        jack = new GUI::Jack(GUI::Jack::OUTPUT);
        jack->GPlace({0.35, 0.8});
        jack->GSetWidth(0.3);
        GAttach(jack);

        rate = new GUI::AnalogEncoder("rate", 4, 0, 32);
        rate->GPlace({0.15, 0.4});
        rate->GSetWidth(0.7);
        GAttach(rate);

        plot = new GUI::Plot<GUI::TimeGraph>(30);
        plot->GPlace({0.01, 0.01});
        plot->GSetWidth(0.98);
        plot->GSetHeight(0.98*0.3);
        GAttach(plot);

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
            return plot;
        });

        plot->GSetTapEndCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
            type ++;
            if (type > 3) type = 0;
            return nullptr;
        });
    }

    void MRender(double beat) override {
        phase += (beat - last_beat) * *rate;
        last_beat = beat;
        *jack = osc(phase);
        if (phase > 6.283f) phase -= 6.283f;
        if (counter > (sample_rate / 30)) {
            plot->update(*jack);
            counter = 0;
        }
        counter ++;
        jack->MRender(beat);
        rate->MRender(beat);
    }

    inline void MIn(MData cmd) override {
        rate->MIn(cmd);
        MOut(cmd);
    }

    struct BLFO {
        uint8_t id = 0x01;
        uint8_t rate_int = 0;
        uint8_t rate_frac = 0;
        uint8_t value = 0;
    };

    inline uint8_t get_int(float val) {
        return static_cast<uint8_t>(val);
    }

    inline uint8_t get_frac(float val) {
        return static_cast<uint8_t>((int)(*rate * 100) % 100);
    }

    int BRender (uint8_t * data) override {
        BLFO blfo;
        blfo.rate_int = get_int(*rate);
        blfo.rate_frac = get_frac(*rate);
        blfo.value = static_cast<uint8_t>((*jack + 1) / 2 * 127);
        std::memcpy(data, &blfo, sizeof(BLFO));
        return sizeof(BLFO);
    }

};


#endif //PD_LFO_H
