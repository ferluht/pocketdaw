//
// Created by Admin on 12.06.2019.
//

#ifndef PD_MASTER_H
#define PD_MASTER_H

#include <ableton/Link.hpp>
#include <GUI/Canvas.h>
#include <GUI/vectordisplay/vectordisplay.h>
#include <GUI/Encoder.h>
#include <GUI/Button.h>
#include <GUI/Menu.h>
#include <Instruments/Oscillator.h>
#include <Instruments/Metronome.h>
#include <Instruments/SingleTone.h>
#include <AudioEffects/Delay.h>
#include <MidiEffects/Arpeggiator.h>
#include <AudioEffects/Oscilloscope.h>
#include <AudioEffects/MoogFilter.h>
#include <AudioEffects/StereoDelay.h>
#include <Instruments/Sampler.h>
#include <Instruments/DrumRack.h>
#include <Instruments/AnalogDrums/Kick.h>
#include <Instruments/AnalogDrums/Snare.h>
#include <Instruments/Brute.h>
#include <AudioEffects/ConvolutionReverb.h>
#include <MidiEffects/Sequencer.h>
#include "Track.h"

class AMGMasterTrack : public GUI::AMGCanvas{

private:

    double last_phase = 0;
    int focus_track = -1;

public:

    ableton::Link link;
    unsigned char size_denominator;
    double bpm, beat, phase;
    bool isPlaying;
    GUI::ProgressButton * linkButton;
    GUI::Button * metronome_button;
    Metronome * metronome;
    GUI::Menu * addMenu;
    GUI::Menu * addDeviceMenu;
    GUI::Menu * addMidiMenu;
    GUI::Menu * addAudioMenu;
//    Oscilloscope * masterWaveform;
//
    std::vector<AMGTrack*> Tracks;
    AMGChain AEffects;

    AMGMasterTrack() : link(120.0) {
        link.enable(true);
//        GAttachTexture("Textures/background.bmp");
        size_denominator = 4;
        isPlaying = false;
        bpm = 120;
        linkButton = new GUI::ProgressButton("Link", [this](bool state){
            this->link.enable(state);
            isPlaying = state;
        });
        metronome = new Metronome();
        metronome_button = new GUI::Button("Metr", [](bool state){});
        beat = 0;
        phase = 0;

//        auto tr = new Arpeggiator();
//        tr->place(0, 0);
//        tr->setHeight(0.5);
//        tr->setRatio(1);
//        GAttach(tr);

        addMenu = new GUI::Menu("Add");
        GAttach(addMenu);

        addDeviceMenu = new GUI::Menu("Instrument");
        GAttach(addDeviceMenu);
        addAudioMenu = new GUI::Menu("Audio device");
        GAttach(addAudioMenu);
        addMidiMenu = new GUI::Menu("Midi device");
        GAttach(addMidiMenu);

        addMenu->addButton(new GUI::FocusButton("Instrument", [this](bool state){}, addDeviceMenu));
        addMenu->addButton(new GUI::FocusButton("Audio device", [this](bool state){}, addAudioMenu));
        addMenu->addButton(new GUI::FocusButton("Midi device", [this](bool state){}, addMidiMenu));

        addMenu->addButton(new GUI::Button("Track", [this](bool a){
                                                        std::string tracknum = std::to_string(Tracks.size());
                                                        auto tr = new AMGTrack(tracknum);
                                                        AddTrack(tr);
                                                    }));

        addDeviceMenu->addButton(new GUI::Button("Oscillator",
                               [this](bool a){
                                   if (focus_track > -1) {
                                       Tracks[focus_track]->RAdd(new Oscillator("Oscillator", 1));
                                   }
                               }));

        addDeviceMenu->addButton(new GUI::Button("SingleTone",
                                                 [this](bool a){
                                                     if (focus_track > -1) {
                                                         Tracks[focus_track]->RAdd(new SingleTone());
                                                     }
                                                 }));

        addDeviceMenu->addButton(new GUI::Button("Sampler",
                                                 [this](bool a){
                                                     if (focus_track > -1) {
                                                         Tracks[focus_track]->RAdd(new Sampler("/storage/emulated/0/test.wav"));
                                                     }
                                                 }));

        addDeviceMenu->addButton(new GUI::Button("Analog kick",
                                                 [this](bool a){
                                                     if (focus_track > -1) {
                                                         Tracks[focus_track]->RAdd(new Kick());
                                                     }
                                                 }));

        addDeviceMenu->addButton(new GUI::Button("Analog snare",
                                                 [this](bool a){
                                                     if (focus_track > -1) {
                                                         Tracks[focus_track]->RAdd(new Snare());
                                                     }
                                                 }));

        addDeviceMenu->addButton(new GUI::Button("Brute",
                                                 [this](bool a){
                                                     if (focus_track > -1) {
                                                         auto br = new Brute();
                                                         Tracks[focus_track]->RAdd(br);
                                                     }
                                                 }));

        addDeviceMenu->addButton(new GUI::Button("Drum rack",
                                                 [this](bool a){
                                                     if (focus_track > -1) {
                                                         auto drack = new DrumRack();
                                                         drack->addSample("/storage/emulated/0/808.wav", 36);
                                                         drack->addSample("/storage/emulated/0/Kick.wav", 37);
                                                         drack->addSample("/storage/emulated/0/Snare.wav", 38);
                                                         drack->addSample("/storage/emulated/0/ClosedHH.wav", 39);
                                                         drack->addSample("/storage/emulated/0/Shaker.wav", 40);
                                                         Tracks[focus_track]->RAdd(drack);
                                                     }
                                                 }));

        auto tr = new AMGTrack(std::string("0"));
        AddTrack(tr);
//        Tracks[focus_track]->RAdd(new Sampler("/storage/emulated/0/test.wav"));

//        addDeviceMenu->addItem(L"Operator",
//                               [this](){
//                                   if (focus_track > -1) {
//                                       Tracks[focus_track]->RAttachInsrument(new Operator(4));
//                                   }
//                               });
//
//        addDeviceMenu->addItem(L"Samler",
//                               [this](){
//                                   if (focus_track > -1) {
//                                       Tracks[focus_track]->RAttachInsrument(new Sampler("/storage/emulated/0/808.wav"));
//                                   }
//                               });
//
//        addDeviceMenu->addItem(L"drack",
//                               [this](){
//                                   if (focus_track > -1) {
//                                       auto drack = new DrumRack();
//                                       drack->addSample("/storage/emulated/0/808.wav", 36);
//                                       drack->addSample("/storage/emulated/0/Kick.wav", 37);
//                                       drack->addSample("/storage/emulated/0/Snare.wav", 38);
//                                       drack->addSample("/storage/emulated/0/ClosedHH.wav", 39);
//                                       drack->addSample("/storage/emulated/0/Shaker.wav", 40);
//                                       Tracks[focus_track]->RAttachInsrument(drack);
//                                   }
//                               });
//
//        addDeviceMenu->addItem(L"ST",
//                               [this](){
//                                   if (focus_track > -1) {
//                                       Tracks[focus_track]->RAttachInsrument(new SingleTone());
//                                   }
//                               });
//
//        addMidiMenu = new Menu(L"Midi effect");
        addMidiMenu->addButton(new GUI::Button("Arp",
                             [this](bool a){
                                 if (focus_track > -1) {
                                     Tracks[focus_track]->RAdd(new Arpeggiator());
                                 }
                             }));

        addMidiMenu->addButton(new GUI::Button("Sequencer",
                                               [this](bool a){
                                                   if (focus_track > -1) {
                                                       Tracks[focus_track]->RAdd(new Sequencer(4));
                                                   }
                                               }));
//
//        addAudioMenu = new Menu(L"Audio effect");
//        addAudioMenu->addItem(L"Oscill",
//                              [this](){
//                                  if (focus_track > -1) {
//                                      Tracks[focus_track]->RAddAudioEffect(new Oscilloscope());
//                                  }
//                              });
//

        addAudioMenu->addButton(new GUI::Button("Convolution reverb",
                                                [this](bool a){
                                                    if (focus_track > -1) {
                                                        Tracks[focus_track]->RAdd(new ConvolutionReverb("/storage/emulated/0/impulse_response_left.wav"));
                                                    }
                                                }));

        addAudioMenu->addButton(new GUI::Button("Delay",
                              [this](bool a){
                                  if (focus_track > -1) {
                                      Tracks[focus_track]->RAdd(new Delay());
                                  }
                              }));

        addAudioMenu->addButton(new GUI::Button("Stereo delay",
                                                [this](bool a){
                                                    if (focus_track > -1) {
                                                        Tracks[focus_track]->RAdd(new StereoDelay());
                                                    }
                                                }));

        addAudioMenu->addButton(new GUI::Button("Oscilloscope",
                                               [this](bool a){
                                                   if (focus_track > -1) {
                                                       Tracks[focus_track]->RAdd(new Oscilloscope());
                                                   }
                                               }));

        addAudioMenu->addButton(new GUI::Button("Moog filter",
                                                [this](bool a){
                                                    if (focus_track > -1) {
                                                        Tracks[focus_track]->RAdd(new MoogFilter());
                                                    }
                                                }));

//        addAudioMenu->addItem(L"Filter",
//                              [this](){
//                                  if (focus_track > -1) {
//                                      Tracks[focus_track]->RAddAudioEffect(new MoogFilter());
//                                  }
//                              });
//
//        addMenu->addSubmenu(L"Device", addDeviceMenu);
//        addMenu->addSubmenu(L"Midi", addMidiMenu);
//        addMenu->addSubmenu(L"Audio", addAudioMenu);
//
//        masterWaveform = new Oscilloscope();
//        masterWaveform->NoHeader();
//
//        std::string tracknum = std::to_string(Tracks.size());
//        auto tr = new AMGTrack(tracknum);
//        AddTrack(tr);

//        if (focus_track > -1) {
//            auto drack = new DrumRack();
//            drack->addSample("/storage/emulated/0/808.wav", 36);
//            drack->addSample("/storage/emulated/0/Kick.wav", 37);
//            drack->addSample("/storage/emulated/0/Snare.wav", 38);
//            drack->addSample("/storage/emulated/0/ClosedHH.wav", 39);
//            drack->addSample("/storage/emulated/0/Shaker.wav", 40);
//            Tracks[focus_track]->RAttachInsrument(drack);
//        }

//        if (focus_track > -1) {
//            Tracks[focus_track]->RAddAudioEffect(new Oscilloscope());
//        }

//        midiDeviceMenu->GSetVisible(true);
//        auto tr2 = new AMGTrack();
//        AddTrack(tr2);
    }

    bool ARender(float * audioData, int numFrames) override;

    void AddTrack(AMGTrack * track) {
        Tracks.push_back(track);
        changeTrackFocus(Tracks.size() - 1);
        track->lPlace({0, 0.4});
        track->lSetHeight(0.6);
        track->lSetWidth(1);
        GAttach(track);

//        track->mc->place(0.4, 0.01, 0.1);
//        track->mc->setHeight(0.38);
//        track->mc->setWidth(0.59);
//        GAttach(track->mc);
    }

    void changeTrackFocus(int i){
        if (focus_track > -1) {
            Tracks[focus_track]->GSetVisible(false);
//            Tracks[focus_track]->mc->GSetVisible(false);
        }
        if (i > (int)Tracks.size() - 1) i = Tracks.size() - 1;
        if (i < 0) i = 0;
        focus_track = i;
        Tracks[focus_track]->GSetVisible(true);
//        Tracks[focus_track]->mc->GSetVisible(true);
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
