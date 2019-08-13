//
// Created by Admin on 09.08.2019.
//

#ifndef PD_SYNTH_H
#define PD_SYNTH_H

#include <GUI/Button.h>
#include "GUI/Canvas.h"
#include "Orchestration/Track.h"

class Synth : public AMGCanvas {

private:

    int focus_track;
    AMGMasterTrack * Master;
    Button * mapping_mode;
    MGObject * mapping_object;

public:

    Synth(){
        Master = new AMGMasterTrack();
        GAttachTexture("Textures/background.bmp");
        place(0, 0, 1, 1);

        Master->place(0, 0.05, 1, 1);
        GAttach(Master);
        MConnect(Master);

        mapping_mode = new Button(L"MIDI MAP", [this](bool state){
            MEnableMapping(state);
        });
        mapping_mode->place(0.89, 0, 0.05, 0.11);
        GAttach(mapping_mode);
    }

    inline void ARender(float * audioData, int numFrames) override {
        Master->ARender(audioData, numFrames);
    }

    void MIn(MData cmd) override {
        if (*mapping_mode && mapping_object) {
            mapping_object->MMap(cmd);
        }
        MOut(cmd);
    }

    GObject * GFindFocusObject(const ndk_helper::Vec2& point) override {
        auto object = AMGCanvas::GFindFocusObject(point);
        if (*mapping_mode) {
            mapping_object = dynamic_cast<MGObject *> (object);
        }
        return object;
    }

};

#endif //PD_SYNTH_H
