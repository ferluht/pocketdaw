//
// Created by ibelikov on 23.12.19.
//

#ifndef PD_ANALOGDRUM_H
#define PD_ANALOGDRUM_H


#include <Instruments/Instrument.h>
#include <GUI/Encoder.h>
#include <Instruments/Envelopes/AD.h>

template <class ADState>
class AnalogDrum : public Instrument<ADState> {

public:

    GUI::Button * trig;

    AD button_light_env;

    AnalogDrum(const char * name_) : Instrument<ADState>(1, name_) {
        this->GSetRatio(0.4);
        this->trig = new GUI::Button(name_, [this] (bool state) {});
        this->trig->GPlace({0.05, 0.75});
        this->trig->GSetHeight(0.2);
        this->trig->GSetWidth(0.9);
        this->GAttach(this->trig);
        this->MConnect(this->trig);

        this->button_light_env.A = 0.01;
        this->button_light_env.D = 0.3;
    }

    void placeEncoder(GUI::Encoder * enc, int posx, int posy) {
        enc->GPlace({0.05f + 0.5f * posx, 0.25f * posy});
        enc->GSetHeight(0.2);
        this->GAttach(enc);
        this->MConnect(enc);
    }

    virtual void MRender(double beat) override {
        if (*trig) {
            this->MIn({beat, NOTEON_HEADER, 62, 100});
            trig->state = false;
        }

        float opacity = 1;

        this->button_light_env.ARender(beat, &opacity, &opacity);

        if(opacity < 0.1f) {
            trig->lightOff();
        } else {
            trig->lightOn(YELLOW, opacity);
        }
    }

    void IUpdateState(ADState *state, MData md) override {
        if (((md.status & 0xF0) == NOTEON_HEADER) && (md.data2 != 0)) {
            button_light_env.attack(md.beat);
        }
    }

};


#endif //PD_ANALOGDRUM_H
