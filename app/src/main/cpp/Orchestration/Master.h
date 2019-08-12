//
// Created by Admin on 12.06.2019.
//

#ifndef PD_MASTER_H
#define PD_MASTER_H

#include <stdlib.h>
#include <vector>
#include <ableton/Link.hpp>

#include "Orchestration/Track.h"
#include "Orchestration/Midi.h"
#include "AudioEffects/AudioEffect.h"
#include "GUI/Canvas.h"
#include "Engine/Engine.h"

#define DEFAULT_BPM 60.

class Master : public AMGObject, public Canvas{

public:

    std::atomic<float> wave[2000];

    int k = 0;

    float prev_sample = 0;

    std::vector<Track*> Tracks;
    std::vector<AudioEffect*> AudioEffects;

    unsigned int focusTrack;

    Master();

    void render(float *audioData, int32_t numFrames);
    void addTrack(Track* track);
    void delTrack(int pos);
    void addAudioEffect(AudioEffect* effect);
    void delAudioEffect(int pos);

    void setSampleRate(int samplerate);
    void start();
    void stop();

    // MIDI methods
    void receiveMIDI(MidiData md);

    ableton::Link link;
    bool rec;
    double sample_rate;
    double bpm, beat, phase;
    unsigned char size_numerator, size_denominator;
    bool isPlaying;
    unsigned char stopPressed_;

};


#endif //PD_MASTER_H
