//
// Created by Admin on 12.06.2019.
//

#ifndef PD_MASTER_H
#define PD_MASTER_H

#include <ableton/Link.hpp>
#include <GUI/Canvas.h>
#include <GUI/Button.h>
#include <Instruments/Metronome.h>
#include <GUI/Menu.h>
#include <AudioEffects/Oscilloscope.h>
#include "Track.h"
#include <Utils/Utils.h>
#include <Instruments/SingleTone.h>
#include <AudioEffects/Delay.h>
#include <AudioEffects/MoogFilter.h>
#include <Instruments/Sampler.h>
#include <Instruments/DrumRack.h>

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
    Menu * addAudioMenu;
    Oscilloscope * masterWaveform;

    std::vector<AMGTrack*> Tracks;
    AMGChain AEffects;

    AMGMasterTrack() : link(120.0) {
        link.enable(true);
        GAttachTexture("Textures/background.bmp");
        size_denominator = 4;
//        isPlaying = true;
        bpm = 120;
        linkButton = new ProgressButton(L"Link", [this](bool state){
            this->link.enable(state);
            isPlaying = state;
        });
        metronome = new Metronome();
        metronome_button = new Button(L"Metr", [](bool state){});
        beat = 0;
        phase = 0;

        addMenu = new Menu(L"Add");

//        auto tr = new Arpeggiator();
//        tr->place(0, 0);
//        tr->setHeight(0.5);
//        tr->setRatio(1);
//        GAttach(tr);

        addDeviceMenu = new Menu(L"Device");

        addDeviceMenu->addItem(L"Oscillator",
                               [this](){
                                   if (focus_track > -1) {
                                       Tracks[focus_track]->RAttachInsrument(new Oscillator(L"Oscillator", 1));
                                   }
                               });

        addDeviceMenu->addItem(L"Operator",
                               [this](){
                                   if (focus_track > -1) {
                                       Tracks[focus_track]->RAttachInsrument(new Operator(4));
                                   }
                               });

        addDeviceMenu->addItem(L"Samler",
                               [this](){
                                   if (focus_track > -1) {
                                       Tracks[focus_track]->RAttachInsrument(new Sampler("/storage/emulated/0/808.wav"));
                                   }
                               });

        addDeviceMenu->addItem(L"drack",
                               [this](){
                                   if (focus_track > -1) {
                                       auto drack = new DrumRack();
                                       drack->addSample("/storage/emulated/0/808.wav", 36);
                                       drack->addSample("/storage/emulated/0/Kick.wav", 37);
                                       drack->addSample("/storage/emulated/0/Snare.wav", 38);
                                       drack->addSample("/storage/emulated/0/ClosedHH.wav", 39);
                                       drack->addSample("/storage/emulated/0/Shaker.wav", 40);
                                       Tracks[focus_track]->RAttachInsrument(drack);
                                   }
                               });

        addDeviceMenu->addItem(L"ST",
                               [this](){
                                   if (focus_track > -1) {
                                       Tracks[focus_track]->RAttachInsrument(new SingleTone());
                                   }
                               });

        addMidiMenu = new Menu(L"Midi effect");
        addMidiMenu->addItem(L"Arp",
                             [this](){
                                 if (focus_track > -1) {
                                     Tracks[focus_track]->RAddMidiEffect(new Arpeggiator());
                                 }
                             });

        addAudioMenu = new Menu(L"Audio effect");
        addAudioMenu->addItem(L"Oscill",
                              [this](){
                                  if (focus_track > -1) {
                                      Tracks[focus_track]->RAddAudioEffect(new Oscilloscope());
                                  }
                              });

        addAudioMenu->addItem(L"Del",
                              [this](){
                                  if (focus_track > -1) {
                                      Tracks[focus_track]->RAddAudioEffect(new Delay());
                                  }
                              });

        addAudioMenu->addItem(L"Filter",
                              [this](){
                                  if (focus_track > -1) {
                                      Tracks[focus_track]->RAddAudioEffect(new MoogFilter());
                                  }
                              });

        addMenu->addSubmenu(L"Device", addDeviceMenu);
        addMenu->addSubmenu(L"Midi", addMidiMenu);
        addMenu->addSubmenu(L"Audio", addAudioMenu);
        addMenu->addItem(L"Track",
                         [this](){
                             std::string tracknum = std::to_string(Tracks.size());
                             auto tr = new AMGTrack(tracknum);
                             AddTrack(tr);
                         });

        masterWaveform = new Oscilloscope();
        masterWaveform->NoHeader();

        std::string tracknum = std::to_string(Tracks.size());
        auto tr = new AMGTrack(tracknum);
        AddTrack(tr);

//        if (focus_track > -1) {
//            auto drack = new DrumRack();
//            drack->addSample("/storage/emulated/0/808.wav", 36);
//            drack->addSample("/storage/emulated/0/Kick.wav", 37);
//            drack->addSample("/storage/emulated/0/Snare.wav", 38);
//            drack->addSample("/storage/emulated/0/ClosedHH.wav", 39);
//            drack->addSample("/storage/emulated/0/Shaker.wav", 40);
//            Tracks[focus_track]->RAttachInsrument(drack);
//        }

        if (focus_track > -1) {
            Tracks[focus_track]->RAddAudioEffect(new Oscilloscope());
        }

//        midiDeviceMenu->GSetVisible(true);
//        auto tr2 = new AMGTrack();
//        AddTrack(tr2);
    }

    bool ARender(float * audioData, int numFrames) override;

    void AddTrack(AMGTrack * track) {
        Tracks.push_back(track);
        changeTrackFocus(Tracks.size() - 1);
        track->place(0, 0.4);
        track->setHeight(0.6);
        track->setWidth(1);
        GAttach(track);

        track->mc->place(0.4, 0.01, 0.1);
        track->mc->setHeight(0.38);
        track->mc->setWidth(0.59);
        GAttach(track->mc);
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
        cmd.beat = beat;
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


#endif //PD_MASTER_H
