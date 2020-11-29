//
// Created by Admin on 12.06.2019.
//

#ifndef PD_TRACK_H
#define PD_TRACK_H

#include <stdlib.h>
#include <map>
#include <vector>
#include <queue>
#include <AMGEngine.h>
#include "GUI/Canvas.h"
#include "GUI/Button.h"
#include "Chain.h"
#include "Rack.h"
#include "Midi.h"
#include <GUI/Menu.h>
#include <Utils/Utils.h>

class AMGTrack : public AMGRack {

public:

    std::vector<MidiClip *> patterns;
    int focus_pattern;

    vecmath::Vec2 drag_from;
    bool mc_visible;
    std::thread * track_thread;
    std::mutex run_lock;
    std::mutex stop_lock;
    std::mutex end_lock;
    std::mutex destroy_lock;
    int counter = 0;
    int divider = 1;

    double beat;
    double increment;
    const float * inputData;
    int inputFrames;
    float * outputData;
    int outputFrames;
    bool pattern_switch = false;
    bool input_enabled = false;

    AMGTrack(std::string name) : AMGRack() {
        drag_from = {0, 0};

        for (int i = 0; i < 4; i++) {
            patterns.push_back(new MidiClip());
            patterns[i]->MConnect(m_in);

            patterns[i]->GPlace({0, 0});
            patterns[i]->GSetWidth(1);
            GAttach(patterns[i]);
            patterns[i]->GSetVisible(false);
        }

        focus_pattern = 0;
        patterns[focus_pattern]->GSetVisible(mc_visible);

        TSetMCHeight(0.23);

        setColor(GUI::GEngine::ui_theme->BACKGROUND_COLOR);

        run_lock.lock();
        stop_lock.unlock();
        end_lock.unlock();
        destroy_lock.lock();
        track_thread = new std::thread(AMGTrackRun, this);
    }

    void TSetMCHeight(float height) {
        mc_visible = height != 0;
        for (int i = 0; i < 4; i++) {
            patterns[i]->GSetHeight(height);
            patterns[i]->GSetVisible(false);
        }
        patterns[focus_pattern]->GSetVisible(mc_visible);
        obj_space_c = {0, height};
        obj_space_s = {1, 1 - height};
        updatePositions();
    }

    inline void MIn(MData cmd) override {
        if ((cmd.status == 0xB0) && (cmd.data1 == 100)) {
            for (int i = 0; i < 4; i++) patterns[i]->record = cmd.data2 > 0;
            return;
        }
        if ((cmd.status == 0xB0) && (cmd.data1 == 0x13)) {
            pattern_switch = cmd.data2 > 0;
        }
        if (pattern_switch && ((cmd.status & 0xF0) == NOTEON_HEADER) && cmd.data1 > 35 && cmd.data1 < 41) {
            if (cmd.data2 > 0) {
                patterns[focus_pattern]->GSetVisible(false);
                patterns[cmd.data1 - 36]->MCHotRestart();
                focus_pattern = cmd.data1 - 36;
                patterns[focus_pattern]->GSetVisible(mc_visible);
            }
            return;
        }
//        if ((cmd.status == 0xB0) && (cmd.data1 == 104) && (cmd.data2 > 0)) {
//            if (cmd.data2 > 0 && cmd.data2 < 5) {
//                patterns[focus_pattern]->GSetVisible(false);
//                patterns[cmd.data2 - 1]->MCHotRestart();
//                patterns[focus_pattern]->MCCopyTo(patterns[cmd.data2 - 1]);
//                focus_pattern = cmd.data2 - 1;
//                patterns[focus_pattern]->GSetVisible(mc_visible);
//            }
//            return;
//        }
        patterns[focus_pattern]->MIn(cmd);
    }

    inline void MRender(double beat) override {
        patterns[focus_pattern]->MRender(beat);
        AMGRack::MRender(beat);
    }

    void GSetVisible(bool visible_) override {
        GObject::GSetVisible(visible_);
        for (int i = 0; i < 4; i++) patterns[i]->GSetVisible(false);
        patterns[focus_pattern]->GSetVisible(visible_);
    }

    void AMGTrackProcess(double beat_, double increment_, const float * inputData_, int inputFrames_, float * outputData_, int outputFrames_) {
        stop_lock.lock();
        beat = beat_;
        increment = increment_;
        inputData = inputData_;
        inputFrames = inputFrames_;
        outputData = outputData_;
        outputFrames = outputFrames_;

        stop_lock.unlock();
        end_lock.lock();
        run_lock.unlock();
    }

    void AMGTrackWait() {
        end_lock.lock();
        end_lock.unlock();
    }

    void GRender(NVGcontext *nvg, float dTime) override {

        if (counter == 0) {
            auto lmidi = &MEngine::getMEngine();
            uint8_t data[100] = {0xF0, 0x01, static_cast<char> (objects.size())};
            int numbytes = 3;
            for (auto const &obj : objects) {
                numbytes += obj->BRender(&data[numbytes]);
            }
            lmidi->sendMidi(data, 0, numbytes, 0);
        }
        counter = (counter + 1) % divider;
    }

    static void AMGTrackRun(AMGTrack * track) {
        while(!track->destroy_lock.try_lock()) {
            track->run_lock.lock();
            track->stop_lock.lock();

            for (int i = 0; i < track->outputFrames; i++) {
                if (track->input_enabled) {
                    track->outputData[2 * i] = 0.5f * (track->inputData[2 * i] + track->inputData[2 * i + 1]);
                    track->outputData[2 * i + 1] = 0.5f * (track->inputData[2 * i] + track->inputData[2 * i + 1]);
                } else {
                    track->outputData[2 * i] = 0;
                    track->outputData[2 * i + 1] = 0;
                }

                track->MRender(track->beat);

                track->ARender(track->beat, &track->outputData[2 * i], &track->outputData[2 * i + 1]);

                track->beat += track->increment;

                track->MRender(track->beat);
            }

            track->stop_lock.unlock();
            track->end_lock.unlock();
        }
    }

    ~AMGTrack(){
        destroy_lock.unlock();
    }
};


#endif //PD_TRACK_H
