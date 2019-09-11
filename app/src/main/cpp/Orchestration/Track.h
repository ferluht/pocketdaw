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
#include <Utils/Utils.h>

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

    void Rearrange() {
        float cur_ratio = 0;
        for (auto const &obj : AMGObjects) {
            obj->place(cur_ratio/ratio, 0);
            obj->setHeight(1);
            cur_ratio += obj->ratio;
        }
        changed = true;
    }

    inline void AMGChainPushBack(AMGObject * mo) {
        auto size = AMGObjects.size();
        if (size > 1) {
            AMGObjects[size - 2]->MDisconnect(AMGObjects.back());
            AMGObjects[size - 2]->MConnect(mo);
        }
        mo->MConnect(AMGObjects.back());
        setRatio(ratio + mo->ratio);
        AMGObjects.insert(AMGObjects.end() - 1, mo);
        GAttach(mo);
        Rearrange();
    }

    inline void AMGChainDel(int pos) {
        auto size = AMGObjects.size();
        setRatio(ratio - AMGObjects[pos]->ratio);
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
        Rearrange();
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

    float padding = 0.01;

    MObject mout;

public:

    AMGChain MEffects;
    AMGObject * Instr;
    AMGChain AEffects;

    AMGRack(){
        Instr = new AMGObject();
        MEffects.MConnect(Instr);
        MEffects.MConnect(&mout);
        Instr->MConnect(&AEffects);

        GAttachTexture("Textures/track_canvas.bmp");

        GAttach(&MEffects);
        GAttach(&AEffects);
    }

    inline void RAttachInsrument(AMGObject * instr_) {
        MEffects.MDisconnect(Instr);
        Instr->MDisconnect(&AEffects);
        GDetach(Instr);
        delete Instr;

        Instr = instr_;
        GAttach(Instr);

        Instr->MConnect(&AEffects);
        MEffects.MConnect(Instr);
        Rearrange();
    }

    void Rearrange(){
        MEffects.place(padding, padding);
        MEffects.setHeight(1 - 2*padding);
        Instr->place((MEffects.ratio + padding)/globalPosition.ratio + padding, padding);
        Instr->setHeight(1 - 2*padding);
        AEffects.place((MEffects.ratio + Instr->ratio + 2*padding)/globalPosition.ratio + padding, padding);
        AEffects.setHeight(1 - 2*padding);
    }

    inline void RDetachInsrument() {
        MEffects.MDisconnect(Instr);
        Instr->MDisconnect(&AEffects);
        Instr = new AMGObject();
        GDetach(Instr);
        MEffects.MConnect(Instr);
        Instr->MConnect(&AEffects);
    }

    inline void RAddMidiEffect(AMGObject * meffect){
        MEffects.AMGChainPushBack(meffect);
        Rearrange();
    }

    inline void RDelMidiEffect(){

    }

    inline void MIn(MData cmd) override {
        MEffects.MIn(cmd);
    }

    inline void MEnableMapping(bool state) override {
        MEffects.MEnableMapping(state);
        Instr->MEnableMapping(state);
        AEffects.MEnableMapping(state);
    }

    inline void MRender(double beat) override {
        MEffects.MRender(beat);
        Instr->MRender(beat);
        AEffects.MRender(beat);
    }

//    void MConnect(MObject * mo) override ;
//    void MDisconnect(MObject * mo) override ;

    void ARender(double beat, float * lsample, float * rsample) override ;
};

class MidiNote : public MData, public GCanvas{

public:

    double end_beat;

    MidiNote() {
        GAttachTexture("Textures/track_canvas.bmp");
    }

    MidiNote& operator=(const MData &cmd)
    {
        status = cmd.status;
        data1 = cmd.data1;
        data2 = cmd.data2;
        beat = cmd.beat;
        end_beat = cmd.beat;
        return *this;
    }

    void MNSetEnd(double beat_) {
        end_beat = beat_;
    }
};

class MidiClip : public MGCanvas{

public:

    std::list<MidiNote> Notes;

    double length;
    bool record;
    bool record_automation;
    std::list<MidiNote>::iterator position;

    std::map<unsigned char, MidiNote *> unfinished_notes;

    MidiClip(){
        GAttachTexture("Textures/midi_canvas.bmp");
        length = 4;

        record = true;
        record_automation = false;
        position = Notes.begin();
    }

    void MIn(MData cmd) override {
        if (record) {
            if ((cmd.status == NOTEON_HEADER) || (cmd.status == NOTEOFF_HEADER)) {
                if (cmd.data2){
                    auto note = new MidiNote();
                    *note = cmd;
                    GAttach(note);
                    unfinished_notes.insert({cmd.data1, note});
                } else {
                    auto note = unfinished_notes.find(cmd.data1)->second;
                    note->MNSetEnd(cmd.beat);
                    note->place(note->beat / length, (float)note->data1/50.0);
                    note->setWidth(abs(note->beat - note->end_beat) / length);
                    note->setHeight(1.0/50.0);
                    unfinished_notes.erase(cmd.data1);
                }
            } else if (record_automation) {

            }
        }
        MOut(cmd);
    }

    void MRender(double beat) override {
//        for (auto const& pair : unfinished_notes) {
//            auto note = pair.second;
//            note->place(note->beat / length, note->data1/50);
//            note->setWidth((note->beat - note->end_beat) / length);
//            note->setHeight(1.0/50.0);
//        }
//        if (beat > position->) ;
    }

//    void GDraw() override;
};

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

//    void GDragHandler(const ndk_helper::Vec2& v) override ;
//    void GDragBegin(const ndk_helper::Vec2& v) override ;
};

class AMGMasterTrack : public AMGCanvas{

private:

    double last_phase = 0;
    int focus_track = -1;

public:

    ableton::Link link;
    unsigned char size_denominator;
    double bpm, beat, phase;
    bool isPlaying;
    ProgressButton * linkButton;
    Button * metronome_button;
    Metronome * metronome;
    Menu * addMenu;
    Menu * addDeviceMenu;
    Menu * addMidiMenu;
    Waveform * masterWaveform;

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

        addMenu = new Menu(L"Add");

//        auto tr = new Arpeggiator();
//        tr->place(0, 0);
//        tr->setHeight(0.5);
//        tr->setRatio(1);
//        GAttach(tr);

        addDeviceMenu = new Menu(L"Device");
        addDeviceMenu->addItem(L"Operator",
                               [this](){
                                   if (focus_track > -1) {
                                       Tracks[focus_track]->RAttachInsrument(new Operator(L"Operator", 8));
                                   }
                               });

        addMidiMenu = new Menu(L"Midi effect");
        addMidiMenu->addItem(L"Arpeggiator",
                               [this](){
                                   if (focus_track > -1) {
                                       Tracks[focus_track]->RAddMidiEffect(new Arpeggiator());
                                   }
                               });

        addMenu->addSubmenu(L"Device", addDeviceMenu);
        addMenu->addSubmenu(L"Midi", addMidiMenu);
        addMenu->addItem(L"Track",
                [this](){
                    std::string tracknum = std::to_string(Tracks.size());
                    auto tr = new AMGTrack(tracknum);
                    AddTrack(tr);
                });

        masterWaveform = new Waveform(50);
//        midiDeviceMenu->GSetVisible(true);
//        auto tr2 = new AMGTrack();
//        AddTrack(tr2);
    }
    void ARender(float * audioData, int numFrames) override;

    void AddTrack(AMGTrack * track) {
        Tracks.push_back(track);
        changeTrackFocus(Tracks.size() - 1);
        track->place(0, 0.5);
        track->setHeight(0.5);
        track->setWidth(1);
        GAttach(track);

        track->mc->place(0.4, 0.01, 0.1);
        track->mc->setHeight(0.48);
        track->mc->setWidth(0.59);
        GAttach(track->mc);
//        MConnect(track);
    }

    void changeTrackFocus(int i){
        if (focus_track > -1) {
            Tracks[focus_track]->GSetVisible(false);
            Tracks[focus_track]->mc->GSetVisible(false);
        }
        if (i > (int)Tracks.size() - 1) i = Tracks.size() - 1;
        if (i < 0) i = 0;
        focus_track = i;
        Tracks[focus_track]->GSetVisible(true);
        Tracks[focus_track]->mc->GSetVisible(true);
    }

    inline void MIn(MData cmd) override{
        cmd.beat = phase;
        if (cmd.status == 0xb0 && cmd.data2 > 0){
            switch (cmd.data1){
                case 0x16:
                    changeTrackFocus(focus_track - 1);
                    break;
                case 0x15:
                    changeTrackFocus(focus_track + 1);
                    break;
                default:
                    break;
            }
        }
        if (focus_track > -1) Tracks[focus_track]->MIn(cmd);
    }

    inline void MRender(double beat) override {
        for (auto const& track : Tracks) track->MRender(beat);
    }

//    void GDragHandler(const ndk_helper::Vec2& v) override ;
//    void GDragBegin(const ndk_helper::Vec2& v) override ;
};


#endif //PD_TRACK_H
