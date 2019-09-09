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
#include <ableton/Link.hpp>
#include <Instruments/Metronome.h>
#include <Instruments/Operator.h>
#include <AudioEffects/Waveform.h>
#include <MidiEffects/Arpeggiator.h>
#include <GUI/Menu.h>

class AMGChain : public AMGObject {

private:

    std::vector<AMGObject*> AMGObjects;

public:

    AMGChain() {
        auto dummy = new AMGObject();
        AMGObjects.push_back(dummy);
    }

    inline void ARender(double beat, float * lsample, float * rsample) override {
        for (auto const &ao : AMGObjects) {
            ao->ARender(beat, lsample, rsample);
        }
    }

    inline void MIn(MData cmd) override {
        AMGObjects.front()->MIn(cmd);
    }

    inline void MConnect(MObject * mo) override {
        AMGObjects.back()->MConnect(mo);
    }

    inline void MDisconnect(MObject * mo) override {
        AMGObjects.back()->MDisconnect(mo);
    }

    inline void AMGChainPushBack(AMGObject * mo) {
        auto size = AMGObjects.size();
        if (size > 1) {
            AMGObjects[size - 2]->MDisconnect(AMGObjects.back());
            AMGObjects[size - 2]->MConnect(mo);
        }
        mo->MConnect(AMGObjects.back());
        if (size == 1) {
            mo->place(0, 0);
            mo->setHeight(1);
        } else {
            mo->place(AMGObjects[size - 2]->x + 0.02, 0);
            mo->setHeight(1);
        }
        AMGObjects.insert(AMGObjects.end() - 1, mo);
        GAttach(mo);
    }

    inline void AMGChainDel(int pos) {
        auto size = AMGObjects.size();
        if (pos > 0 && pos < size - 1) {
            AMGObjects[pos]->MDisconnect(AMGObjects[pos + 1]);
            AMGObjects[pos - 1]->MDisconnect(AMGObjects[pos]);
            AMGObjects[pos - 1]->MConnect(AMGObjects[pos + 1]);
            GDetach(AMGObjects[pos]);
            AMGObjects.erase(AMGObjects.begin() + pos);
        } else if (pos == 0 && size > 1) {
            AMGObjects[0]->MDisconnect(AMGObjects[1]);
            GDetach(AMGObjects[pos]);
            AMGObjects.erase(AMGObjects.begin() + pos);
        }
    }

    inline void MRender(double beat) override {
        for (auto const& mo : AMGObjects) mo->MRender(beat);
    }

    inline void MEnableMapping(bool state) override {
        for (auto const& mo : AMGObjects) mo->MEnableMapping(state);
    }

};

class AMGRack : public AMGCanvas{

private:

    AMGObject dummy_instr;
    MObject mout;

public:

    AMGChain MEffects;
    AMGObject * Instr;
    AMGChain AEffects;

    AMGRack(){
        Instr = &dummy_instr;
        MEffects.MConnect(Instr);
        MEffects.MConnect(&mout);
        Instr->MConnect(&AEffects);

        GAttachTexture("Textures/effect_canvas.bmp");
        place(0, 0);
        setHeight(1);
        setWidth(1);
    }

    inline void RAttachInsrument(AMGObject * instr_) {
        Instr = instr_;
        GAttach(Instr);

        Instr->place(0.2, 0);
        Instr->setHeight(1);

        AEffects.place(0.6, 0);
        AEffects.setHeight(1);
        AEffects.setRatio(2);
        MEffects.place(0.1, 0);
        MEffects.setHeight(1);
        AEffects.setRatio(2);

        GAttach(&MEffects);
        GAttach(&AEffects);

        MEffects.MDisconnect(&dummy_instr);
        dummy_instr.MDisconnect(&AEffects);
        MEffects.MConnect(Instr);
        Instr->MConnect(&AEffects);
    }

    inline void RDetachInsrument() {
        MEffects.MDisconnect(Instr);
        Instr->MDisconnect(&AEffects);
        Instr = &dummy_instr;
        GDetach(Instr);
        MEffects.MConnect(Instr);
        Instr->MConnect(&AEffects);
    }

    inline void MIn(MData cmd) override {
        MEffects.MIn(cmd);
    }

    inline void MEnableMapping(bool state) override {
        MEffects.MEnableMapping(state);
        Instr->MEnableMapping(state);
        AEffects.MEnableMapping(state);
    }

//    void MConnect(MObject * mo) override ;
//    void MDisconnect(MObject * mo) override ;

    void ARender(double beat, float * lsample, float * rsample) override ;
};


class AMGTrack : public AMGCanvas{

public:

    MGObject * focusObject;
    MGObject * trackMenu;
    AMGRack Rack;

    AMGTrack(){
        Rack.RAttachInsrument(new Operator(8));
        GAttach(&Rack);
        GAttachTexture("Textures/track_canvas.bmp");
        Rack.place(0.02, 0.01);
        Rack.setWidth(0.98);
        Rack.setHeight(0.98);
        MConnect(&Rack);
        Rack.AEffects.AMGChainPushBack(new Waveform(200));

//        Rack.MEffects.AMGChainPushBack(new Arpeggiator());
    }
    ~AMGTrack(){}

    inline void ARender(double beat, float *lsample, float *rsample) override {
        Rack.ARender(beat, lsample, rsample);
    }
//    void GDragHandler(const ndk_helper::Vec2& v) override ;
//    void GDragBegin(const ndk_helper::Vec2& v) override ;
};

class AMGMasterTrack : public AMGCanvas{

private:

    double last_phase = 0;

public:

    ableton::Link link;
    unsigned char size_denominator;
    double bpm, beat, phase;
    bool isPlaying;
    ProgressButton * linkButton;
    Button * metronome_button;
    Metronome * metronome;
    Menu * addDeviceMenu;

    std::vector<AMGTrack*> Tracks;
    AMGChain AEffects;

    AMGMasterTrack() : link(120.0) {
        link.enable(true);
        GAttachTexture("Textures/background.bmp");
        size_denominator = 4;
        isPlaying = true;

        linkButton = new ProgressButton(L"Link", [this](bool state){ this->link.enable(state); });
        metronome = new Metronome();
        metronome_button = new Button(L"Metr", [](bool state){});
        beat = 0;

        addDeviceMenu = new Menu(L"Add device");
        addDeviceMenu->addItem(L"Operator",
                [this](){
                    auto tr = new AMGTrack();
                    AddTrack(tr);
                });
//        midiDeviceMenu->GSetVisible(true);
//        auto tr2 = new AMGTrack();
//        AddTrack(tr2);
    }

    void ARender(float * audioData, int numFrames) override;

    void AddTrack(AMGTrack * track) {
        Tracks.push_back(track);
        track->place(0, 0.5);
        track->setHeight(0.5);
        track->setWidth(1);
        GAttach(track);
        MConnect(track);
    }

//    void GDragHandler(const ndk_helper::Vec2& v) override ;
//    void GDragBegin(const ndk_helper::Vec2& v) override ;
};


#endif //PD_TRACK_H
