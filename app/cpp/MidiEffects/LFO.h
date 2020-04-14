//
// Created by ibelikov on 01.01.20.
//

#ifndef PD_LFO_H
#define PD_LFO_H


#include <GUI/Jack.h>
#include <GUI/Encoder.h>
#include <GUI/Graph.h>
#include <GUI/Plot.h>
#include "MidiEffect.h"

class LFO : public MidiEffect {

    GUI::Jack * jack;
    GUI::AnalogEncoder * rate;
    GUI::Plot<GUI::TimeGraph> * plot;

    int counter = 0;

    double phase = 0;
    double last_beat = 0;

public:

    LFO() : MidiEffect("LFO") {
        GSetRatio(0.3);
        jack = new GUI::Jack(GUI::Jack::OUTPUT);
        jack->GPlace({0.35, 0.8});
        jack->GSetWidth(0.3);
        GAttach(jack);

        rate = new GUI::AnalogEncoder("rate", 4, 0, 32);
        rate->GPlace({0.15, 0.4});
        rate->GSetWidth(0.7);
        GAttach(rate);

        plot = new GUI::Plot<GUI::TimeGraph>(30);
        plot->GPlace({0.01, 0.01});
        plot->GSetWidth(0.98);
        plot->GSetHeight(0.98*0.3);
        GAttach(plot);
    }

    void MRender(double beat) override {
        phase += (beat - last_beat) * *rate;
        last_beat = beat;
        *jack = sin(phase);
        if (phase > 6.283f) phase -= 6.283f;
        if (counter > (sample_rate / 30)) {
            plot->update(*jack);
            counter = 0;
        }
        counter ++;
        jack->MRender(beat);
        rate->MRender(beat);
    }

};


#endif //PD_LFO_H
