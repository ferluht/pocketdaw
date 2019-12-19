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

    int step;

    int focus_drum = 0;

    static const int STEPS = 16;

public:


    AMGCanvas * upper_panel;
    AMGCanvas * drum_panel[SOUNDS];
    GUI::Button * seq_buttons[SOUNDS][STEPS];


    Brute() : InstrumentBase("brute") {
        shape->setRatio(0.4 * SOUNDS);

        step = 0;

        drums[0] = new Kick();
        drums[0]->NoHeader();
        drums[0]->shape->lPlace({0, 0});
        drums[0]->shape->lSetHeight(1);
        GAttach(drums[0]);

        drums[1] = new Snare();
        drums[1]->NoHeader();
        drums[1]->shape->lPlace({0.5, 0});
        drums[1]->shape->lSetHeight(1);
        GAttach(drums[1]);

        upper_panel = new AMGCanvas();


        for (int j = 0; j < SOUNDS; j++) {

            drum_panel[j] = new AMGCanvas();

            for (int i = 0; i < STEPS; i++){
                seq_buttons[j][i] = new GUI::Button(std::to_string(i).c_str(), [](bool a) {});
                seq_buttons[j][i]->shape->lPlace({1.0f / (float) STEPS * i, 0.1});
                seq_buttons[j][i]->shape->setRatio(1);
                seq_buttons[j][i]->shape->lSetWidth(1.0f / (float) STEPS);
                drum_panel[j]->GAttach(seq_buttons[j][i]);
            }
            drum_panel[j]->shape->lPlace({0,0});
            drum_panel[j]->shape->lSetWidth(1);
            drum_panel[j]->shape->lSetHeight(1);
            drum_panel[j]->GSetVisible(false);
            upper_panel->GAttach(drum_panel[j]);
        }

        drum_panel[0]->GSetVisible(true);
    }

    void focusOnDrum(int i) {
        seq_buttons[focus_drum][step]->lightOff();
        for (auto const & panel : drum_panel) panel->GSetVisible(false);
        drum_panel[i]->GSetVisible(true);
        focus_drum = i;
    }

    virtual GObject *GFindFocusObject(const Vec2 &point, std::list<GObject *> * trace) {
        for (int i = 0; i < SOUNDS; i++) {
            if (drums[i]->visible && drums[i]->shape->contains(point)) focusOnDrum(i);
        }
        return AMGCanvas::GFindFocusObject(point, trace);
    }

    void MIn(MData cmd) override {
        if ((cmd.status & 0xF0) == NOTEON_HEADER) {
            switch (cmd.data1) {
                case 36:
                    drums[0]->MIn(cmd);
                    focusOnDrum(0);
                    break;
                case 37:
                    drums[1]->MIn(cmd);
                    focusOnDrum(1);
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

        auto newstep = (int)(beat*4)%16;
        if (newstep != step) {
            seq_buttons[focus_drum][step]->lightOff();
            step = newstep;
            seq_buttons[focus_drum][step]->lightOn(RED);
            for (int i = 0; i < 2; i ++)
                if (*seq_buttons[i][step]) drums[i]->MIn({beat, NOTEON_HEADER, 62, 100});
        }
    }

    bool ARender(double beat, float * lsample, float * rsample) override {
        for (auto const & drum : drums) drum->ARender(beat, lsample, rsample);
        return true;
    }
};


#endif //PD_BRUTE_H
