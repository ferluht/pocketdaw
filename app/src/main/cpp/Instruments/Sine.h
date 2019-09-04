//
// Created by Admin on 09.08.2019.
//

#ifndef PD_SINE_H
#define PD_SINE_H

#include "Instruments/Instrument.h"
#include "ADSR.h"
#include <GUI/Encoder.h>
#include <GUI/Graph.h>

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

class Sine : public Instrument<SineState>{

    Encoder * enc_coarse;
    Encoder * enc_fine;
    Encoder * enc_level;
    Encoder * enc_attack;
    Encoder * enc_decay;
    Encoder * enc_sustain;
    Encoder * enc_release;

    float A, D, S, R;

    TimeGraph * graph;

    int graph_phase;

    float k;

    double phase_shift;
    float ratio;
    float fine;

public:

    float level;

    Sine(unsigned int num_voices);

    void IUpdateState(SineState * state, MData md) override;

    void IARender(SineState * state, double beat, float * lsample, float * rsample) override ;
};


#endif //PD_SINE_H
