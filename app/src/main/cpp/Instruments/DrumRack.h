//
// Created by Admin on 20.09.2019.
//

#ifndef PD_DRUMRACK_H
#define PD_DRUMRACK_H

#include <Orchestration/Rack.h>
#include "Instrument.h"
#include "Sampler.h"

class DrumRackState : public InstrumentState{

};

class DrumRack : public Instrument<DrumRackState> {

    std::map<int, AMGRack *> chains;
    AMGRack * focus_chain;

public:

    DrumRack() : Instrument(1, L"drack") {
        setRatio(2);
        focus_chain = nullptr;
    }

    void addSample(const char * sample_name_, const char note) {
        auto chain = new AMGRack();
        chain->place(0.51, 0.01);
        chain->setHeight(0.98);
        chain->setWidth(0.48);
        GAttach(chain);
        MConnect(chain);
        focusOn(chain);
        chain->RAttachInsrument(new Sampler(sample_name_));
        chains.insert({note, chain});
    }

    void focusOn(AMGRack * chain){
        if (focus_chain) focus_chain->GSetVisible(false);
        focus_chain = chain;
        if (chain) chain->GSetVisible(true);
    }

    void MIn(MData cmd) override {
        switch (cmd.status & 0xF0){
            case NOTEON_HEADER:
            case NOTEOFF_HEADER:
                {
                    auto chain = chains.find(cmd.data1);
                    if (chain != chains.end()){
                        chain->second->MIn(cmd);
                        focusOn(chain->second);
                    }
                    break;
                }
            default:
                MOut(cmd);
        }
    }

    bool ARender(double beat, float * lsample, float * rsample) override {
        for (auto const& chain : chains) chain.second->ARender(beat, lsample, rsample);
        return true;
    }

    void GSetVisible(bool visible_) {
        Instrument::GSetVisible(visible_);
        for (auto const& chain : chains) chain.second->GSetVisible(false);
        if (focus_chain) focus_chain->GSetVisible(visible_);
    }
};


#endif //PD_DRUMRACK_H
