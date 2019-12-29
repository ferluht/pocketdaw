//
// Created by ibelikov on 23.12.19.
//

#ifndef PD_SEQUENCER_H
#define PD_SEQUENCER_H


#include <GUI/Canvas.h>
#include <GUI/Button.h>
#include <GUI/IECanvas.h>
#include <GUI/Encoder.h>
#include <GUI/EncoderButton.h>
#include <GUI/Jack.h>
#include "MidiEffect.h"


class SequencerStep : public GUI::AMGCanvas {

public:

    GUI::EncoderButton * trig;

    SequencerStep(const char * name_) {
        trig = new GUI::EncoderButton(name_);
        trig->GPlace({0.05f, 0.02f});
        trig->GSetRatio(1);
        trig->GSetWidth(0.9f);
        GAttach(trig);
    }

};

//class SequencerStep : public GUI::Button {
//
//public:
//
//    SequencerStep() : GUI::Button (std::to_string(0).c_str(), [](bool a) {}) { }
//
//};

class SequencerPattern : public GUI::AMGCanvas {

    std::vector<SequencerStep *> steps;

public:

    SequencerPattern (int n_steps_=64) {

        for (int i = 0; i < n_steps_; i++) {
            char buffer[64];
            snprintf(buffer, sizeof buffer, "step %d", i);
            auto step = new SequencerStep(buffer);
            steps.push_back(step);
            step->GPlace({1.0f / (float) n_steps_ * i, 0.02f});
            step->GSetRatio(1);
            step->GSetWidth(1.0f / (float) n_steps_);
            GAttach(step);
        }
    }

    inline SequencerStep * get(int i) {
        return steps[i];
    }

    inline void set(SequencerStep * st) {}

};

class SequencerChannel : public GUI::AMGCanvas {

    int focus_pattern;
    std::vector<SequencerPattern *> patterns;

public:

    unsigned char mapping;

    SequencerChannel (int n_patterns_, int n_steps_) {
        for (int i = 0; i < n_patterns_; i++) {
            auto pattern = new SequencerPattern(n_steps_);
            patterns.push_back(pattern);
            pattern->GPlace({0, 0});
            pattern->GSetWidth(1);
            pattern->GSetHeight(1);
            GAttach(pattern);
            pattern->GSetVisible(false);
        }

        focus_pattern = 0;
    }

    inline SequencerStep * get(int pattern_, int step_) {
        return patterns[pattern_]->get(step_);
    }

    inline void map(unsigned char mapping_) {
        mapping = mapping_;
    }

    inline void selectPattern(int pattern_) {
        patterns[focus_pattern]->GSetVisible(false);
        focus_pattern = pattern_;
        patterns[focus_pattern]->GSetVisible(true);
    }

    virtual void GSetVisible(bool visible_) override {
        GUI::AMGCanvas::GSetVisible(false);
        if (visible_) {
            selectPattern(focus_pattern);
            visible = true;
        }
    }

};

class Sequencer : public MidiEffect {

    std::vector<SequencerChannel *> channels;
    int focus_channel;
    int focus_pattern;
    int step;

    GUI::TapButton * next;
    GUI::TapButton * prev;

    GUI::Jack * jack;
    GUI::Jack * jack1;

public:

    Sequencer(int n_channels_, int n_steps_=16, int n_patterns_=8) : MidiEffect("Sequencer") {

        GSetRatio(2);

        for (int i = 0; i < n_channels_; i++) {
            auto channel = new SequencerChannel(n_patterns_, n_steps_);
            channels.push_back(channel);
            channel->GPlace({0, 0});
            channel->GSetWidth(1);
            channel->GSetHeight(0.5);
            GAttach(channel);
            channel->GSetVisible(false);
            channel->map(36 + i);
        }

        jack = new GUI::Jack();
        jack->GPlace({0.6, 0.6});
        jack->GSetHeight(0.05);
        GAttach(jack);

        jack1 = new GUI::Jack();
        jack1->GPlace({0.8, 0.6});
        jack1->GSetHeight(0.1);
        GAttach(jack1);


        prev = new GUI::TapButton("prev", [this](bool a){selectChannel(focus_channel - 1);});
        prev->GPlace({0.05, 0.7});
        prev->GSetHeight(0.2);
        prev->GSetRatio(2);
        GAttach(prev);

        next = new GUI::TapButton("next", [this](bool a){selectChannel(focus_channel + 1);});
        next->GPlace({0.25, 0.7});
        next->GSetHeight(0.2);
        next->GSetRatio(2);
        GAttach(next);

        step = 0;
        focus_channel = 0;
        focus_pattern = 0;
        selectChannel(focus_channel);
    }

    inline void selectChannel(int channel_) {
        if (channel_ > ((int)channels.size() - 1)) channel_ = channels.size() - 1;
        if (channel_ < 0) channel_ = 0;
        channels[focus_channel]->GSetVisible(false);
        channels[focus_channel]->get(focus_pattern, step)->trig->lightOff();
        focus_channel = channel_;
        channels[focus_channel]->GSetVisible(true);
        channels[focus_channel]->selectPattern(focus_pattern);
    }

    virtual void GSetVisible(bool visible_) {
        MidiEffect::GSetVisible(false);
        if (visible_) {
            selectChannel(focus_channel);
            visible = true;
        }
    }

    void MRender(double beat) override {
        auto newstep = (int)(beat*4)%16;
        if (newstep != step) {
            channels[focus_channel]->get(focus_pattern, step)->trig->lightOff();
            step = newstep;
            auto button = channels[focus_channel]->get(focus_pattern, step)->trig;
            button->lightOn(RED);
            for (const auto &channel: channels) {
                button = channel->get(focus_pattern, step)->trig;
                if (*button) {
                    MOut({beat, NOTEON_HEADER, channel->mapping, (unsigned char)((float)(*button) * 64 + 64)});
                }
            }
        }
    }

};


#endif //PD_SEQUENCER_H
