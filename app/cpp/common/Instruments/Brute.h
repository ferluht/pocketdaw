//
// Created by ibelikov on 13.12.19.
//

#ifndef PD_BRUTE_H
#define PD_BRUTE_H

#include <common/Instruments/AnalogDrums/Kick.h>
#include <common/Instruments/AnalogDrums/Snare.h>
#include "Instrument.h"

class Brute : public InstrumentBase{

    static const int SOUNDS = 2;

    IECanvas * drums[SOUNDS];

public:

    Brute() : InstrumentBase("brute") {
        setRatio(0.4 * SOUNDS);

        drums[0] = new Kick();
        drums[0]->NoHeader();
        drums[0]->lPlace({0, 0});
        drums[0]->lSetHeight(1);
        GAttach(drums[0]);

        drums[1] = new Snare();
        drums[1]->NoHeader();
        drums[1]->lPlace({0.5, 0});
        drums[1]->lSetHeight(1);
        GAttach(drums[1]);
    }

    virtual GObject *GFindFocusObject(const Vec2 &point, std::list<GObject *> * trace) {
//        for (int i = 0; i < SOUNDS; i++) {
//            if (drums[i]->visible && drums[i]->contains(point)) focusOnDrum(i);
//        }
        return AMGCanvas::GFindFocusObject(point, trace);
    }

    void MIn(MData cmd) override {
        if ((cmd.status & 0xF0) == NOTEON_HEADER) {
            switch (cmd.data1) {
                case 36:
                    drums[0]->MIn(cmd);
//                    focusOnDrum(0);
                    break;
                case 37:
                    drums[1]->MIn(cmd);
//                    focusOnDrum(1);
                    break;
                default:
                    break;
            }
        } else {
            for (auto const & drum : drums) drum->MIn(cmd);
        }
        MOut(cmd);
    }

    void MRender(double beat) override {
        for (auto const & drum : drums) drum->MRender(beat);
    }

    bool ARender(double beat, float * lsample, float * rsample) override {
        for (auto const & drum : drums) drum->ARender(beat, lsample, rsample);
        return true;
    }
};


#endif //PD_BRUTE_H
