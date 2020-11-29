//
// Created by ibelikov on 11.07.19.
//

#ifndef PD_WAVEFORM_H
#define PD_WAVEFORM_H

#include "AudioEffect.h"
#include <GUI/Plot.h>

class Oscilloscope : public AudioEffect{

    GUI::Encoder * trig;
    GUI::Encoder * time;
    GUI::Encoder * scale;

    int sample_counter = 0;
    int after_trig = 0;
    const int graph_points = 100;
    float max = 0;

    GUI::Plot<GUI::TimeGraph> * plot;

    float avg = 0, avg_env = 0;

public:

    Oscilloscope();

    inline float envelope(float sample, float w, float w_env){
        avg = w*sample + (1-w)*avg;
        float i = std::abs(sample - avg);
        avg_env = w_env*i + (1-w_env)*avg_env;
        return avg_env;
    }

    bool ARender(double beat, float * lsample, float * rsample) override ;

//    void pinchBegin(const vecmath::Vec2& v) override ;
//    void pinchHandler(const vecmath::Vec2& v) override ;
//    virtual void pinchEnd() {};

//    void grender(float dTime) override ;
//    void dragBegin(const vecmath::Vec2& v) override ;
//    void dragHandler(const vecmath::Vec2& v) override ;
//    void dragEnd() override ;
};


#endif //PD_WAVEFORM_H
