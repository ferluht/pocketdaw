//
// Created by Admin on 13.08.2019.
//

#ifndef PD_OPERATOR_H
#define PD_OPERATOR_H

#include "Instruments/Instrument.h"
#include "ADSR.h"
#include "Sine.h"
#include <GUI/Encoder.h>
#include <GUI/Graph.h>

class OperatorState : public InstrumentState{
public:
    double beat;
    unsigned char velocity;

    SineState sinestates[4];

    float volume;
    double frequency;
    double phase_increment;

//    void setPhaseActive(int i, bool isactive_) {
//        phase_isactive[i] = isactive_;
//        setActive(phase_isactive[0] || phase_isactive[1] || phase_isactive[2] || phase_isactive[3]);
//    }
};

class Operator : public Instrument<OperatorState>{

    Sine * sines[4];

    TimeGraph * graph;

    int graph_phase;

    Encoder * enc_mode;

    float k;

    unsigned int mode;

    int op_focus;

    float level;

    Text * opname;

public:

    Operator(const wchar_t * name, unsigned int num_voices);

    void IUpdateState(OperatorState * state, MData md) override;

    void IARender(OperatorState * state, double beat, float * lsample, float * rsample) override ;

    void MIn(MData cmd) override ;

    void ARender(double beat, float * lsample, float * rsample) override ;
};



#endif //PD_OPERATOR_H
