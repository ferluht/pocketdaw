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
    Button * metronome_button;
    ProgressButton * linkButton;
    MGObject * mapping_object;
    Menu * midiDeviceMenu;
    Led * midiLeds[2];

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

        metronome_button = Master->metronome_button;
        metronome_button->place(0.69, 0);
        metronome_button->setHeight(0.05);
        metronome_button->setRatio(3);
        GAttach(metronome_button);

        linkButton = Master->linkButton;
        linkButton->place(0.79, 0);
        linkButton->setHeight(0.05);
        linkButton->setRatio(3);
        GAttach(linkButton);

        mapping_mode = new Button(L"MIDI", [this](bool state){ MEnableMapping(state); });
        mapping_mode->place(0.89, 0);
        mapping_mode->setHeight(0.05);
        mapping_mode->setRatio(3);
        GAttach(mapping_mode);

        midiLeds[0] = new Led(false);
        midiLeds[0]->place(0.001, 0.001);
        midiLeds[0]->setHeight(0.0235);
        GAttach(midiLeds[0]);

        midiLeds[1] = new Led(true);
        midiLeds[1]->place(0.001, 0.0274);
        midiLeds[1]->setHeight(0.0235);
        GAttach(midiLeds[1]);

        midiDeviceMenu = new Menu(L"MIDI device");
        midiDeviceMenu->place(0.1, 0);
        midiDeviceMenu->setHeight(0.05);
        midiDeviceMenu->setRatio(6);
//        midiDeviceMenu->GSetVisible(true);
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
        midiLeds[0]->toggle();
        midiLeds[1]->toggle();
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
