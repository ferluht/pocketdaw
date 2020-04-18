//
// Created by Admin on 09.08.2019.
//

#ifndef PD_SYNTH_H
#define PD_SYNTH_H

#include <GUI/Button.h>
#include <GUI/Menu.h>
#include <GUI/Led.h>
#include <Orchestration/Master.h>
#include <GUI/Canvas.h>
#include <time.h>

class Synth : public GUI::AMGCanvas {

private:

    int focus_track;
    AMGMasterTrack * Master;
    GUI::Button * mapping_mode;
    GUI::Button * metronome_button;
    GUI::Button * device_menu;
    GUI::ProgressButton * linkButton;
    AMGObject * mapping_object;
    GUI::Menu * midiDeviceMenu;
    GUI::Button * midi_device_menu;
    GUI::Button * record_button;
    GUI::Button * save_button;
    GUI::ProgressButton * cpuload;
    GUI::Led * midiLeds[2];
//    Oscilloscope * masterWaveform;

    float panel_height = 0.05f;
    float panel_buttons_start_x = 0.06f;
    float panel_button_ratio = 3;
    float panel_button_width = 0.1;
    float mc_height = 0;

    AudioFile<float> audioFile;
    char filename[100];

    double render_time = 0;
    double last_render_time = 0;

    std::list<GUI::Button *> upper_panel_buttons;

    void add_upper_panel_button(GUI::Button * b){
        b->GPlace({panel_buttons_start_x + upper_panel_buttons.size() * panel_button_width, 0});
        b->GSetWidth(panel_button_width);
        b->GSetHeight(panel_height);
        upper_panel_buttons.push_back(b);
        GAttach(b);
    }

public:

    Synth(){

        Master = new AMGMasterTrack();
        Master->GPlace({0, panel_height});
        Master->GSetHeight(1 - panel_height);
        Master->GSetWidth(1);
        GAttach(Master);
        MConnect(Master);

        metronome_button = Master->metronome_button;
        add_upper_panel_button(metronome_button);

        linkButton = Master->linkButton;
        add_upper_panel_button(linkButton);

        mapping_mode = new GUI::Button("MIDI", [this](bool state){
            if (!state && mc_height < 1) {
                mc_height = 1;
                mapping_mode->state = true;
            } else if (!state) {
                mc_height = 0;
            } else {
                mc_height = 0.25;
            }
            Master->MSetMCHeight(mc_height);
        });
        *mapping_mode = true;
        add_upper_panel_button(mapping_mode);

        float led_height = panel_height * 0.4f;
        midiLeds[0] = new GUI::Led(false);
        midiLeds[0]->GPlace({led_height / 2 + 0.001f, led_height / 2 + panel_height * .05f});
        midiLeds[0]->GSetHeight(led_height);
        GAttach(midiLeds[0]);

        midiLeds[1] = new GUI::Led(true);
        midiLeds[1]->GPlace({led_height / 2 + 0.001f, led_height * 1.5f + panel_height * .15f});
        midiLeds[1]->GSetHeight(led_height);
        GAttach(midiLeds[1]);

        device_menu = new GUI::FocusButton("Add", [this](bool state){}, Master->addMenu);
        add_upper_panel_button(device_menu);

        midiDeviceMenu = new GUI::Menu("Midi device");
        GAttach(midiDeviceMenu);


        auto midi = &MEngine::getMEngine();
        auto mnames = midi->getDevices();
        if (mnames.size()) {
            midi->connectDevice(mnames.front());
        }

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

        record_button = new GUI::Button("REC", [this](bool state){
            MOut({0, 0xB0, 100, static_cast<unsigned char> (state * 127)});
        });
        add_upper_panel_button(record_button);

        save_button = new GUI::Button("SAVE", [this](bool state){
            if (state) {
                struct timespec res;
                clock_gettime(CLOCK_REALTIME, &res);
                double t = 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;
                sprintf(filename, "/storage/emulated/0/Music/pocketdaw/%lld.wav", static_cast<long long>(t));

                audioFile.setBitDepth (16);
                audioFile.setSampleRate (static_cast<unsigned int>(sample_rate));
                audioFile.setNumChannels(2);
//                audioFile.setNumSamplesPerChannel(100000);

                audioFile.save (filename);
            }
        });
        add_upper_panel_button(save_button);

        cpuload = new GUI::ProgressButton("CPU", [this](bool state){

        });
        add_upper_panel_button(cpuload);

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
//        midiDeviceMenu->GSetRatio(6);
////        midiDeviceMenu->GSetVisible(true);
//        GAttach(midiDeviceMenu);
//        MConnect(midiDeviceMenu);
//
//        addMenu = Master->addMenu;
//        addMenu->place(0.3, 0);
//        addMenu->setHeight(0.05);
//        addMenu->GSetRatio(6);
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
        bool ret = Master->ARender(audioData, numFrames);

        struct timespec res;
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &res);
//        clock_gettime(CLOCK_THREAD_CPUTIME_ID, )
        render_time = res.tv_sec + (double) res.tv_nsec / 1e9;
        float load = static_cast<float>(render_time - last_render_time) / ((float)numFrames / sample_rate);
        last_render_time = render_time;
        if (load > 1) load = 1;
        cpuload->progress(load);

        if (*save_button) {

            FILE* f= fopen(filename, "ab");

            uint8_t bytes[4*numFrames];
            for (int i = 0; i < numFrames; i++)
            {
                int16_t l = audioFile.sampleToSixteenBitInt (audioData[i*2]);
                int16_t r = audioFile.sampleToSixteenBitInt (audioData[i*2 + 1]);
                bytes[4*i + 3] = (uint8_t) (r >> 8) & 0xFF;
                bytes[4*i + 2] = (uint8_t) r & 0xFF;
                bytes[4*i + 1] = (uint8_t) (l >> 8) & 0xFF;
                bytes[4*i + 0] = (uint8_t) l & 0xFF;
            }

            fwrite(bytes, sizeof(uint8_t), 4*numFrames, f);

            fclose(f);
        }
        return ret;
    }

    void MIn(MData cmd) override {
        midiLeds[0]->toggle();
        midiLeds[1]->toggle();
        if (cmd.status == 0xB0 && cmd.data1 == 100) {
            if (cmd.data2 > 0) record_button->state ^= true;
        }
        MOut(cmd);
    }

    GObject * GFindFocusObject(const ndk_helper::Vec2& point, std::list<GObject *> * trace) override {
        auto object = AMGCanvas::GFindFocusObject(point, trace);
        if (*mapping_mode) {
            mapping_object = dynamic_cast<AMGObject *> (object);
        }
        return object;
    }

    virtual void GDraw(NVGcontext *nvg) {
        nvgBeginPath(nvg);
        nvgRect(nvg,
                global.c.x,
                global.c.y,
                global.s.x, global.s.y * panel_height);
        nvgFillColor(nvg, MIDGREY);
        nvgFill(nvg);
        nvgClosePath(nvg);
    }

};

#endif //PD_SYNTH_H

