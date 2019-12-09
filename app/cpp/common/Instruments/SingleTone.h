//
// Created by Admin on 13.09.2019.
//

#ifndef PD_SINGLETONE_H
#define PD_SINGLETONE_H

#include <GUI/Encoder.h>
#include "Instrument.h"
#include "ADSR.h"

class SingleToneState : public InstrumentState{
public:
    double beat;
    unsigned char velocity;
    float note;

    float glide;
    float glide_inc;
    float glide_dir;

    ADSR adsr;

    float volume;

    float phase1;
    float phase2;
};

class SingleTone : public Instrument<SingleToneState> {

    GUI::Encoder * interp1;
    GUI::Encoder * interp2;

    GUI::Encoder * crossmod;

    GUI::Encoder * A;
    GUI::Encoder * D;
    GUI::Encoder * S;
    GUI::Encoder * R;

    const float encoder_height = 0.25;
    const float encoder_spacing = 0.028;
    const float row_1st = 0.1;

    float glide_time;

public:

    SingleTone();

    void IUpdateState(SingleToneState * state, MData md) override;

    void IARender(SingleToneState * state, double beat, float * lsample, float * rsample) override ;
};


#endif //PD_SINGLETONE_H
