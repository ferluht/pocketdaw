//
// Created by Admin on 12.06.2019.
//

#ifndef PD_MASTER_H
#define PD_MASTER_H

#include <stdlib.h>
#include <vector>
#include "Track.h"
#include "../AudioEffects/AudioEffect.h"
#include <ableton/Link.hpp>
#include "Midi.h"


#define DEFAULT_BPM 60.

class Master {

public:

    std::atomic<float> wave[2000];
    int r = 1;

    float accumulator = 0, prev_acc = 0;
    int ai = 0, aj = 0;
    float window = 3;

    float prev_sample = 0;

    std::vector<Track*> Tracks;
    std::vector<AudioEffect*> AudioEffects;

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
