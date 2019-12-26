//
// Created by Admin on 12.06.2019.
//

#ifndef PD_TRACK_H
#define PD_TRACK_H

#include <stdlib.h>
#include <map>
#include <vector>
#include <queue>
#include "Engine/Engine.h"
#include "GUI/Canvas.h"
#include "GUI/Button.h"
#include "Chain.h"
#include "Rack.h"
//#include "Midi.h"
#include <ableton/Link.hpp>
#include <GUI/Menu.h>
#include <Utils/Utils.h>

class AMGTrack : public AMGRack{

public:

//    MidiClip * mc;

    Vec2 drag_from;

    AMGTrack(std::string name) : AMGRack() {
        drag_from = {0, 0};
//        mc = new MidiClip();
//        mc->MConnect(&MEffects);
    }

//    inline void MIn(MData cmd) override {
////        mc->MIn(cmd);
//    }

    inline void MRender(double beat) override {
//        mc->MRender(beat);
        AMGRack::MRender(beat);
    }

    ~AMGTrack(){}

    void GDraw(NVGcontext *nvg) override {
        nvgBeginPath(nvg);
        nvgRect(nvg,
                global.c.x,
                global.c.y,
                global.s.x, global.s.y);
        nvgFillColor(nvg, DARKER);
        nvgFill(nvg);
        nvgClosePath(nvg);
    };

    GUI::GObject * GDragHandler(const ndk_helper::Vec2& v) override ;
    GUI::GObject * GDragBegin(const ndk_helper::Vec2& v) override ;
};


#endif //PD_TRACK_H
