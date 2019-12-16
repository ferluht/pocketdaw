//
// Created by ibelikov on 11.07.19.
//

#ifndef PD_WAVEFORM_H
#define PD_WAVEFORM_H

#include "AudioEffect.h"
#include <common/GUI/Plot.h>

class Oscilloscope : public AudioEffect{

    GUI::Encoder * trig;
    GUI::Encoder * time;
    GUI::Encoder * scale;

    int sample_counter = 0;
    int after_trig = 0;
    const int graph_points = 200;

    GUI::Plot<GUI::TimeGraph> * plot;

public:

    Oscilloscope();

    bool ARender(double beat, float * lsample, float * rsample) override ;

//    void pinchBegin(const ndk_helper::Vec2& v) override ;
//    void pinchHandler(const ndk_helper::Vec2& v) override ;
//    virtual void pinchEnd() {};

//    void grender(float dTime) override ;
//    void dragBegin(const ndk_helper::Vec2& v) override ;
//    void dragHandler(const ndk_helper::Vec2& v) override ;
//    void dragEnd() override ;
};


#endif //PD_WAVEFORM_H
