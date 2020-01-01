//
// Created by Admin on 20.09.2019.
//

#ifndef PD_DRUMRACK_H
#define PD_DRUMRACK_H

#include <Orchestration/Rack.h>
#include <GUI/Jack.h>
#include <GUI/AnalogEncoder.h>
#include "Instrument.h"
#include "Sampler.h"

class DrumRackState : public InstrumentState{

};

class DrumRack : public Instrument<DrumRackState> {

    AMGRack * notes[128];
    std::list<AMGRack *> activeChains;
    AMGRack * focus_chain;

    GUI::AnalogEncoder * enc;

public:

    DrumRack() : Instrument(1, "Drum rack") {
        GSetRatio(2);
        focus_chain = nullptr;

        for (int i = 0; i < 128; i ++) notes[i] = nullptr;

        enc = new GUI::AnalogEncoder("test");
        enc->GPlace({0.1, 0.5});
        enc->GSetHeight(0.4);
        GAttach(enc);
        MConnect(enc);
    }

    void addSample(const char * sample_name_, const char note) {
        auto chain = new AMGRack();
        chain->GPlace({0.51, 0.01});
        chain->GSetHeight(0.98);
        chain->GSetWidth(0.48);
        GAttach(chain);
        MConnect(chain);
        focusOn(chain);
        chain->RAdd(new Sampler(sample_name_));
        notes[note] = chain;
        activeChains.push_back(chain);
    }

    void focusOn(AMGRack * chain){
        if (focus_chain) focus_chain->GSetVisible(false);
        focus_chain = chain;
        if (chain) chain->GSetVisible(true);
    }

    void MIn(MData cmd) override {
        switch (cmd.status & 0xF0) {
            case NOTEON_HEADER:
            case NOTEOFF_HEADER: {
                if (notes[cmd.data1]) {
                    notes[cmd.data1]->MIn(cmd);
                    focusOn(notes[cmd.data1]);
                }
                break;
            }
            default:
                MOut(cmd);
        }
    }

    bool ARender(double beat, float * lsample, float * rsample) override {
        for (auto const& chain : activeChains) chain->ARender(beat, lsample, rsample);
        return true;
    }

    void GSetVisible(bool visible_) {
        Instrument::GSetVisible(visible_);
        for (auto const& chain : activeChains) chain->GSetVisible(false);
        if (focus_chain) focus_chain->GSetVisible(visible_);
    }

    void MRender(double beat) {
        enc->MRender(beat);
        for (const auto & chain : activeChains) chain->MRender(beat);
    }
};


#endif //PD_DRUMRACK_H
