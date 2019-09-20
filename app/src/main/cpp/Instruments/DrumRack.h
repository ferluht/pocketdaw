//
// Created by Admin on 20.09.2019.
//

#ifndef PD_DRUMRACK_H
#define PD_DRUMRACK_H

#include "Instrument.h"
#include "Sampler.h"

class DrumRackState : public InstrumentState{

};

class DrumRack : public Instrument<DrumRackState> {

    std::vector<Sampler *> samples;

public:

    DrumRack() : Instrument(1, L"drack") {
        setRatio(1);
    }

    void addSample(const char * sample_name_, const char note) {
        auto sample = new Sampler(sample_name_);
        samples.push_back(sample);
        MConnect(sample);
    }

    void MIn(MData cmd) override {
        int index = cmd.data1 - 36;
        switch (cmd.status & 0xF0){
            case NOTEON_HEADER:
            case NOTEOFF_HEADER:
                cmd.data1 += 29;
                if (index > -1 && index < samples.size()) samples[index]->MIn(cmd);
                break;
            default:
                MOut(cmd);
        }
    }

    bool ARender(double beat, float * lsample, float * rsample) override {
        for (auto const& sample : samples) sample->ARender(beat, lsample, rsample);
        return true;
    }
};


#endif //PD_DRUMRACK_H
