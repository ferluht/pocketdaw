//
// Created by ferluht on 18/04/2020.
//

#ifndef PD_SATURATOR_H
#define PD_SATURATOR_H


#include <GUI/Encoder.h>
#include <GUI/Graph.h>
#include <GUI/Plot.h>
#include "AudioEffect.h"

class Saturator : public AudioEffect {

    GUI::AnalogEncoder * drywet;
    GUI::AnalogEncoder * factor;

    GUI::Plot<GUI::TimeGraph> * plot;

    int type = 0;

    bool waveshape_changed = true;

    Vec2 drag_from;

    inline float shaper(float amp) {
        amp *= *factor;
        switch (type){
            case 0:
                amp = 1.f / (1.f + exp(-amp)) * 2 - 1;
                break;
            case 1:
                if (amp > 1) amp = 1;
                if (amp < -1) amp = -1;
                break;
            case 2:
                amp = 1.f / (1.f + abs(amp)) * 2 - 1;
                break;
            case 3:
                break;
            default:
                break;
        }
        return amp;
    }

public:

    Saturator();

    void redraw_wave();
    bool ARender(double beat, float * lsample, float * rsample) override;

    void GDraw(NVGcontext * nvg) override {
        if (waveshape_changed) {
            redraw_wave();
            changed = false;
        }
        AudioEffect::GDraw(nvg);
    }
};


#endif //PD_SATURATOR_H
