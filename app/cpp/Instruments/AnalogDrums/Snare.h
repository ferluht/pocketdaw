//
// Created by ibelikov on 13.12.19.
//

#ifndef PD_SNARE_H
#define PD_SNARE_H

#include "../Instrument.h"
#include "AnalogDrum.h"
#include <GUI/Encoder.h>
#include <GUI/AnalogEncoder.h>
#include <Instruments/Envelopes/AD.h>
#include <random>

class SnareState : public InstrumentState {
public:

    float note;
    double beat;
    unsigned char velocity;

    AD ad;
    AD sweep_env;

    float volume;
    double phase;
    double frequency;
    double phase_increment;

    SnareState () {
        sweep_env.A = 0;
    }
};

class Snare : public AnalogDrum<SnareState> {

    GUI::Encoder * attack;
    GUI::Encoder * tone;
    GUI::Encoder * sweep_time;
    GUI::Encoder * sweep_amt;
    GUI::Encoder * decay;
    GUI::Encoder * waveform;

    std::random_device dev;
    std::mt19937 * rng;
    std::uniform_real_distribution<float> * dist;

    inline double osc(double phase) {
        phase = phase < -M_PI ? M_PI + fmod(phase + M_PI, 2*M_PI) : -M_PI + fmod(phase + M_PI, 2*M_PI);
        float s = -sin(phase), w = 0;
        w = (*dist)(*rng);
        return s * (1 - *waveform) + *waveform * w;
    }

public:

    Snare();

    void IUpdateState(SnareState * state, MData md) override;

    void IARender(SnareState * state, double beat, float * lsample, float * rsample) override ;

};



#endif //PD_SNARE_H
