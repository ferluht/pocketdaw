//
// Created by Admin on 09.08.2019.
//

#ifndef PD_SYNTH_H
#define PD_SYNTH_H

#include "GUI/Canvas.h"
#include "Orchestration/Track.h"

class Synth : public AMGCanvas {

private:

    int focus_track;
    AMGMasterTrack * Master;

public:

    Synth(){
        Master = new AMGMasterTrack();
        GAttachTexture("Textures/background.bmp");
        place(0, 0, 1, 1);
        Master->place(0, 0.05, 1, 1);
        GAttach(Master);
        MConnect(Master);
    }

    inline void ARender(float * audioData, int numFrames) override {
        Master->ARender(audioData, numFrames);
    }

    void MIn(MData cmd) override {
//        if(cmd.status == 0xB0) place(0, 0.1, 1, 1);
        MOut(cmd);
    }

};

#endif //PD_SYNTH_H
