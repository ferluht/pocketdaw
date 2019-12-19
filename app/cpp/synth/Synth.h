//
// Created by Admin on 09.08.2019.
//

#ifndef PD_SYNTH_H
#define PD_SYNTH_H

#include <GUI/Button.h>
#include <GUI/Menu.h>
#include <GUI/Led.h>
#include <Orchestration/Master.h>
//#include <Orchestration/Track.h>
#include <GUI/Canvas.h>

class Synth : public GUI::AMGCanvas {

private:

    int focus_track;
    AMGMasterTrack * Master;
    GUI::Button * mapping_mode;
    GUI::Button * metronome_button;
    GUI::Button * device_menu;
    GUI::ProgressButton * linkButton;
    MGObject * mapping_object;
    GUI::Menu * midiDeviceMenu;
    GUI::Button * midi_device_menu;
    GUI::Led * midiLeds[2];
//    Oscilloscope * masterWaveform;

    float panel_height = 0.05f;
    float panel_buttons_start_x = 0.1f;
    float panel_button_ratio = 3;
    float panel_button_width = 0.1;

    std::list<GUI::Button *> upper_panel_buttons;

    void add_upper_panel_button(GUI::Button * b){
        b->shape->lPlace({panel_buttons_start_x + upper_panel_buttons.size()*panel_button_width, 0});
        b->shape->lSetWidth(panel_button_width);
        b->shape->lSetHeight(panel_height);
        upper_panel_buttons.push_back(b);
        GAttach(b);
    }

public:

    Synth(){
        setShapeType(GUI::BOX);

        Master = new AMGMasterTrack();
        Master->shape->lPlace({0, panel_height});
        Master->shape->lSetHeight(1 - panel_height);
        Master->shape->lSetWidth(1);
        GAttach(Master);
        MConnect(Master);

        metronome_button = Master->metronome_button;
        add_upper_panel_button(metronome_button);

        linkButton = Master->linkButton;
        add_upper_panel_button(linkButton);

        mapping_mode = new GUI::Button("MIDI", [this](bool state){ });
        add_upper_panel_button(mapping_mode);

        float led_height = panel_height * 0.4f;
        midiLeds[0] = new GUI::Led(false);
        midiLeds[0]->shape->lPlace({led_height/2 + 0.001f, led_height/2 + panel_height * .05f});
        midiLeds[0]->shape->lSetHeight(led_height);
        GAttach(midiLeds[0]);

        midiLeds[1] = new GUI::Led(true);
        midiLeds[1]->shape->lPlace({led_height/2 + 0.001f, led_height*1.5f + panel_height * .15f});
        midiLeds[1]->shape->lSetHeight(led_height);
        GAttach(midiLeds[1]);

        device_menu = new GUI::FocusButton("Add", [this](bool state){}, Master->addMenu);
        add_upper_panel_button(device_menu);

        midiDeviceMenu = new GUI::Menu("Midi device");
        GAttach(midiDeviceMenu);

        midi_device_menu = new GUI::FocusButton("Midi device",
                [this](bool state){
                    midiDeviceMenu->clear();
                    auto midi = &MEngine::getMEngine();
                    auto mnames = midi->getDevices();
                    for (auto const& name : mnames) {
                        this->midiDeviceMenu->addButton(
                                new GUI::Button(name.c_str(), [midi, name](bool state){
                                    midi->connectDevice(name);
                                }));
                    }
                }, midiDeviceMenu);
        add_upper_panel_button(midi_device_menu);


//        midiDeviceMenu->setGainCallback([this](){
//            auto midi = &MEngine::getMEngine();
//            auto mnames = midi->getDevices();
//            for (auto const& name : mnames) {
//                std::wstring wide_string = utils::UTF8toUnicode(name);
//                this->midiDeviceMenu->addItem(wide_string.c_str(), [midi, name](){midi->connectDevice(name);});
//            }
//        });
//        midiDeviceMenu->place(0.1, 0);
//        midiDeviceMenu->setHeight(0.05);
//        midiDeviceMenu->setRatio(6);
////        midiDeviceMenu->GSetVisible(true);
//        GAttach(midiDeviceMenu);
//        MConnect(midiDeviceMenu);
//
//        addMenu = Master->addMenu;
//        addMenu->place(0.3, 0);
//        addMenu->setHeight(0.05);
//        addMenu->setRatio(6);
////        midiDeviceMenu->GSetVisible(true);
//        GAttach(addMenu);
//        MConnect(addMenu);
//
//        masterWaveform = Master->masterWaveform;
//        masterWaveform->place(0.5, 0);
//        masterWaveform->setHeight(0.05);
//        masterWaveform->setWidth(0.06);
//        GAttach(masterWaveform);
    }

    inline bool ARender(float * audioData, int numFrames) override {
        return Master->ARender(audioData, numFrames);
    }

    void MIn(MData cmd) override {
        midiLeds[0]->toggle();
        midiLeds[1]->toggle();
        MOut(cmd);
    }

    GObject * GFindFocusObject(const ndk_helper::Vec2& point, std::list<GObject *> * trace) override {
        auto object = AMGCanvas::GFindFocusObject(point, trace);
        if (*mapping_mode) {
            mapping_object = dynamic_cast<MGObject *> (object);
        }
        return object;
    }

    virtual void GDraw(NVGcontext *nvg) {
        nvgBeginPath(nvg);
        nvgRect(nvg,
                shape->global.c.x,
                shape->global.c.y,
                shape->global.s.x, shape->global.s.y * panel_height);
        nvgFillColor(nvg, MIDGREY);
        nvgFill(nvg);
        nvgClosePath(nvg);
    }

};

#endif //PD_SYNTH_H

