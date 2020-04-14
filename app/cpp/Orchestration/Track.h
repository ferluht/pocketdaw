//
// Created by Admin on 12.06.2019.
//

#ifndef PD_TRACK_H
#define PD_TRACK_H

#include <stdlib.h>
#include <map>
#include <vector>
#include <queue>
#include "AMGEngine/AMGEngine.h"
#include "GUI/Canvas.h"
#include "GUI/Button.h"
#include "Chain.h"
#include "Rack.h"
#include "Midi.h"
#include <ableton/Link.hpp>
#include <GUI/Menu.h>
#include <Utils/Utils.h>

class AMGTrack : public AMGRack{

public:

    std::vector<MidiClip *> patterns;
    int focus_pattern;

    Vec2 drag_from;

    AMGTrack(std::string name) : AMGRack() {
        drag_from = {0, 0};

        for (int i = 0; i < 4; i++) {
            patterns.push_back(new MidiClip());
            patterns[i]->MConnect(m_in);

            patterns[i]->GPlace({0, 0.01});
            patterns[i]->GSetHeight(0.23);
            patterns[i]->GSetWidth(1);
            GAttach(patterns[i]);
            patterns[i]->GSetVisible(false);
        }

        focus_pattern = 0;
        patterns[focus_pattern]->GSetVisible(true);

        obj_space_c = {0, 0.25};
        obj_space_s = {1, 0.75};

        GSetDragBeginCallback([this](const Vec2& v) -> GUI::GObject * {
            drag_from = v;
            return this;
        });

        GSetDragHandlerCallback([this](const Vec2& v) -> GUI::GObject * {
            x_offset += (v.x - drag_from.x)/global.s.x;
            if ((max_x_offset > 0) || (x_offset > 0)) x_offset = 0;
            if ((max_x_offset < 0) && (x_offset < max_x_offset)) x_offset = max_x_offset;
            updatePositions();
            drag_from = v;
            return this;
        });

        setColor(DARKER);
    }

    inline void MIn(MData cmd) override {
        if ((cmd.status == 0xB0) && (cmd.data1 == 100)) {
            for (int i = 0; i < 4; i++) patterns[i]->record = cmd.data2 > 0;
            return;
        }
        if ((cmd.status == 0xB0) && (cmd.data1 == 103) && (cmd.data2 > 0)) {
            if (cmd.data2 > 0 && cmd.data2 < 5) {
                patterns[focus_pattern]->GSetVisible(false);
                patterns[cmd.data2 - 1]->MCHotRestart();
                focus_pattern = cmd.data2 - 1;
                patterns[focus_pattern]->GSetVisible(true);
            }
            return;
        }
        if ((cmd.status == 0xB0) && (cmd.data1 == 104) && (cmd.data2 > 0)) {
            if (cmd.data2 > 0 && cmd.data2 < 5) {
                patterns[focus_pattern]->GSetVisible(false);
                patterns[cmd.data2 - 1]->MCHotRestart();
                patterns[focus_pattern]->MCCopyTo(patterns[cmd.data2 - 1]);
                focus_pattern = cmd.data2 - 1;
                patterns[focus_pattern]->GSetVisible(true);
            }
            return;
        }
        patterns[focus_pattern]->MIn(cmd);
    }

    inline void MRender(double beat) override {
        patterns[focus_pattern]->MRender(beat);
        AMGRack::MRender(beat);
    }

    void GSetVisible(bool visible_) override {
        GObject::GSetVisible(visible_);
        for (int i = 0; i < 4; i++) patterns[i]->GSetVisible(false);
        patterns[focus_pattern]->GSetVisible(visible_);
    }

    ~AMGTrack(){}
};


#endif //PD_TRACK_H
