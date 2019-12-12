//
// Created by Admin on 18.09.2019.
//

#ifndef PD_SAMPLER_H
#define PD_SAMPLER_H

#include <Utils/Utils.h>
#include <GUI/Graph.h>
#include <GUI/Encoder.h>
#include "Instrument.h"

class SamplerState : public InstrumentState{
public:

    unsigned char velocity;
    float note;

    float volume;

    bool transient;
    float time;

    float out;

    float alpha;
};

class Sampler : public Instrument<SamplerState> {

    AudioFile<float> sample;
    const char * sample_name;

    GUI::Encoder * pitch;
    bool const_pitch;

    GUI::TimeGraph * graph;

public:

    Sampler(const char * sample_name_);

    float InterpolateCubic(float x0, float x1, float x2, float x3, float t);
    float InterpolateHermite4pt3oX(float x0, float x1, float x2, float x3, float t);

    void IUpdateState(SamplerState * state, MData md) override;

    inline void setConstPitch(bool const_pitch_) {const_pitch = const_pitch_;};

    void IARender(SamplerState * state, double beat, float * lsample, float * rsample) override ;

};


#endif //PD_SAMPLER_H
