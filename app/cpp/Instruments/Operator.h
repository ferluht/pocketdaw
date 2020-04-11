//
// Created by Admin on 13.08.2019.
//

#ifndef PD_OPERATOR_H
#define PD_OPERATOR_H

#include "Instruments/Instrument.h"
#include "Envelopes/ADSR.h"
#include "Oscillator.h"
#include <GUI/Encoder.h>
#include <GUI/Graph.h>

class OperatorState : public InstrumentState{
public:
    double beat;
    unsigned char velocity;
    float note;

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

    const char * algos[11] = {
            "Textures/fm_algos/alg0.bmp",
            "Textures/fm_algos/alg1.bmp",
            "Textures/fm_algos/alg2.bmp",
            "Textures/fm_algos/alg3.bmp",
            "Textures/fm_algos/alg4.bmp",
            "Textures/fm_algos/alg5.bmp",
            "Textures/fm_algos/alg6.bmp",
            "Textures/fm_algos/alg7.bmp",
            "Textures/fm_algos/alg8.bmp",
            "Textures/fm_algos/alg9.bmp",
            "Textures/fm_algos/alg10.bmp"
    };

    Oscillator * sines[4];

    float a = 0, b = 0, c = 0, d = 0;

    GUI::TimeGraph * graph;

    int graph_phase;

    GUI::Encoder * enc_mode;

    float k;

    unsigned int mode;

    int op_focus;

    float level;

    void prevOsc();
    void nextOsc();

//    TexturedMultiButton * algo;

    void setAlgo(unsigned int value);

public:

    Operator(unsigned int num_voices);

    void IUpdateState(OperatorState * state, MData md) override;

    void IARender(OperatorState * state, double beat, float * lsample, float * rsample) override ;

    void MIn(MData cmd) override ;

    GObject * GFindFocusObject(const Vec2 &point, std::list<GObject *> * trace) override
    {
        if (visible && GContains(point)){
            auto focus = Instrument::GFindFocusObject(point, trace);
//            if (focus == sines[op_focus]) {
//                trace->pop_front();
//                return this;
//            }
            return focus;
        }
        return nullptr;
    }

//    GObject * TapEnd(const ndk_helper::Vec2& v) ;

    void GSetVisible(bool visible_) override ;
};



#endif //PD_OPERATOR_H
