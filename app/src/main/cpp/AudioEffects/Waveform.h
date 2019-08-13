//
// Created by ibelikov on 11.07.19.
//

#ifndef PD_WAVEFORM_H
#define PD_WAVEFORM_H

#include "AudioEffect.h"
#include <GUI/Graph.h>

class Waveform : public AudioEffect{
public:

    int r = 1;
    float window = 3;
    float accumulator = 0;
    int ai = 0;

    TimeGraph * graph;

    Waveform(float n);

    void apply(float * lsample, float * rsample) override ;

//    void pinchBegin(const ndk_helper::Vec2& v) override ;
//    void pinchHandler(const ndk_helper::Vec2& v) override ;
//    virtual void pinchEnd() {};

//    void grender(float dTime) override ;
//    void dragBegin(const ndk_helper::Vec2& v) override ;
//    void dragHandler(const ndk_helper::Vec2& v) override ;
//    void dragEnd() override ;
};


#endif //PD_WAVEFORM_H
