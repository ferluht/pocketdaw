//
// Created by ibelikov on 12.12.19.
//

#ifndef PD_KICK_H
#define PD_KICK_H


#include "../Instrument.h"
#include "AnalogDrum.h"
#include <GUI/Jack.h>
#include <Instruments/Envelopes/AD.h>

class KickState : public InstrumentState {
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

    KickState () {
        sweep_env.A = 0;
    }
};

class Kick : public AnalogDrum<KickState> {

    GUI::AnalogEncoder * attack;
    GUI::AnalogEncoder * tone;
    GUI::AnalogEncoder * sweep_time;
    GUI::AnalogEncoder * sweep_amt;
    GUI::AnalogEncoder * decay;
    GUI::AnalogEncoder * waveform;

    inline double osc(double phase) {
        phase = phase < -M_PI ? M_PI + fmod(phase + M_PI, 2*M_PI) : -M_PI + fmod(phase + M_PI, 2*M_PI);
        float s = -sin(phase), w = 0;
        if (phase < 0) w = 1;
        else w = -1;
        return s * (1 - *waveform) + *waveform * w;
    }

public:

    Kick();

    void IUpdateState(KickState * state, MData md) override;

    void IARender(KickState * state, double beat, float * lsample, float * rsample) override ;

    void MRender(double beat) {
        attack->MRender(beat);
        tone->MRender(beat);
        sweep_time->MRender(beat);
        sweep_amt->MRender(beat);
        decay->MRender(beat);
        waveform->MRender(beat);
    }

};


#endif //PD_KICK_H
