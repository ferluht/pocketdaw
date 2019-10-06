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
#include "Midi.h"
#include <ableton/Link.hpp>
#include <Instruments/Metronome.h>
#include <Instruments/Operator.h>
#include <AudioEffects/Oscilloscope.h>
#include <MidiEffects/Arpeggiator.h>
#include <GUI/Menu.h>
#include <Utils/Utils.h>

class AMGTrack : public AMGRack{

public:

    MidiClip * mc;
    Text * trackname;

    AMGTrack(std::string name){
        GAttachTexture("Textures/track_canvas.bmp");

        std::wstring wide_string = utils::UTF8toUnicode(std::string("Track") + name);
        trackname = new Text("Fonts/Roboto-Regular.ttf", wide_string.c_str());
        trackname->place(0, -0.1);
        trackname->setHeight(0.1);
        GAttach(trackname);

        mc = new MidiClip();
        mc->MConnect(&MEffects);
    }

    inline void MIn(MData cmd) override {
        mc->MIn(cmd);
    }

    inline void MRender(double beat) override {
        mc->MRender(beat);
        AMGRack::MRender(beat);
    }

    ~AMGTrack(){}

    GObject * GDragHandler(const ndk_helper::Vec2& v) override ;
    GObject * GDragBegin(const ndk_helper::Vec2& v) override ;
};


#endif //PD_TRACK_H
