//
// Created by Admin on 09.08.2019.
//

#ifndef PD_SYNTH_H
#define PD_SYNTH_H

#include <GUI/Button.h>
#include <GUI/Menu.h>
#include "GUI/Canvas.h"
#include <GUI/Led.h>
#include "Orchestration/Track.h"

class Synth : public AMGCanvas {

private:

    int focus_track;
    AMGMasterTrack * Master;
    Button * mapping_mode;
    MGObject * mapping_object;
    Menu * midiDeviceMenu;
    Led * midiLed;

public:

    Synth(){

        GAttachTexture("Textures/background.bmp");
        place(0, 0);
        setHeight(1);
        setWidth(1);

        Master = new AMGMasterTrack();
        Master->place(0, 0.05);
        Master->setHeight(0.95);
        Master->setWidth(1);
        GAttach(Master);
        MConnect(Master);

        mapping_mode = new Button(L"MIDI MAP", [this](bool state){ MEnableMapping(state); });
        mapping_mode->place(0.89, 0);
        mapping_mode->setHeight(0.05);
        mapping_mode->setRatio(4);
        GAttach(mapping_mode);

        midiLed = new Led();
        midiLed->place(0, 0);
        midiLed->setHeight(0.05);
        GAttach(midiLed);

        std::vector<std::pair<wchar_t *, std::function<void(void)>>> midiDevices;
        midiDeviceMenu = new Menu(midiDevices);
        midiDeviceMenu->place(0.1, 0);
        midiDeviceMenu->setHeight(0.05);
        midiDeviceMenu->setRatio(6);
        midiDeviceMenu->GSetVisible(true);
        GAttach(midiDeviceMenu);
        MConnect(midiDeviceMenu);
    }

    inline void ARender(float * audioData, int numFrames) override {
        Master->ARender(audioData, numFrames);
    }

    void MIn(MData cmd) override {
        if (*mapping_mode && mapping_object) {
            mapping_object->MMap(cmd);
        }
        midiLed->toggle(2);
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
