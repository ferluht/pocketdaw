//
// Created by Admin on 12.06.2019.
//

#ifndef PD_TRACK_H
#define PD_TRACK_H

#include <stdlib.h>
#include <map>
#include <vector>
#include <queue>
#include "AMGEngine/AMGEngine.h"
#include "GUI/Canvas.h"
#include "GUI/Button.h"
#include "Chain.h"
#include "Rack.h"
#include "Midi.h"
#include <ableton/Link.hpp>
#include <GUI/Menu.h>
#include <Utils/Utils.h>

class AMGTrack : public AMGRack {

public:

    std::vector<MidiClip *> patterns;
    int focus_pattern;

    Vec2 drag_from;
    bool mc_visible;
    std::thread * track_thread;
    std::mutex run_lock;
    std::mutex stop_lock;
    std::mutex end_lock;
    std::mutex destroy_lock;

    double beat;
    double increment;
    float * audioData;
    int numFrames;

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

        TSetMCHeight(0.23);

        focus_pattern = 0;
        patterns[focus_pattern]->GSetVisible(mc_visible);

        setColor(DARKER);

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
        if ((cmd.status == 0xB0) && (cmd.data1 == 103) && (cmd.data2 > 0)) {
            if (cmd.data2 > 0 && cmd.data2 < 5) {
                patterns[focus_pattern]->GSetVisible(false);
                patterns[cmd.data2 - 1]->MCHotRestart();
                focus_pattern = cmd.data2 - 1;
                patterns[focus_pattern]->GSetVisible(mc_visible);
            }
            return;
        }
        if ((cmd.status == 0xB0) && (cmd.data1 == 104) && (cmd.data2 > 0)) {
            if (cmd.data2 > 0 && cmd.data2 < 5) {
                patterns[focus_pattern]->GSetVisible(false);
                patterns[cmd.data2 - 1]->MCHotRestart();
                patterns[focus_pattern]->MCCopyTo(patterns[cmd.data2 - 1]);
                focus_pattern = cmd.data2 - 1;
                patterns[focus_pattern]->GSetVisible(mc_visible);
            }
            return;
        }
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

    void AMGTrackProcess(double beat_, double increment_, float * audioData_, int numFrames_) {
        stop_lock.lock();
        beat = beat_;
        increment = increment_;
        audioData = audioData_;
        numFrames = numFrames_;

        stop_lock.unlock();
        end_lock.lock();
        run_lock.unlock();
    }

    void AMGTrackWait() {
        end_lock.lock();
        end_lock.unlock();
    }

    static void AMGTrackRun(AMGTrack * track) {
        while(!track->destroy_lock.try_lock()) {
            track->run_lock.lock();
            track->stop_lock.lock();

            for (int i = 0; i < track->numFrames; i++) {
                track->audioData[2 * i] = 0;
                track->audioData[2 * i + 1] = 0;

                track->MRender(track->beat);

                track->ARender(track->beat, &track->audioData[2 * i], &track->audioData[2 * i + 1]);

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
