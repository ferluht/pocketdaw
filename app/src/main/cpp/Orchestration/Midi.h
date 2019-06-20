//
// Created by Admin on 12.06.2019.
//

#ifndef PD_MIDI_H
#define PD_MIDI_H

#include <stdlib.h>
#include <vector>
#include <queue>
#include <cmath>

#define BASE_NOTE 61.0
#define POWER_BASE 2.0
#define SEMITONES 24.0

class MidiData {

public:

    double beat;
    unsigned char note;
    unsigned char vel;
    MidiData() = default;

};

bool operator<(const MidiData &lhs, const MidiData &rhs);

class MidiClip {

public:

    double begin, pattern_length, loops;
    int end;
//    std::vector<MidiData>::iterator position;
    unsigned int position;

    std::vector<MidiData> Arrangement;

    MidiClip();
    MidiClip(double beat_begin);
    MidiClip(double beat_begin, double pattern_length_);

    void play(std::priority_queue<MidiData> * q, double beat);
    void insertNow(MidiData md);
    void insert(MidiData md, double beat);
    void reset();
    void stop(double beat);
};


MidiClip * createMetronomeMidi();

inline double getFrequency(double base_frequency, unsigned char note){
    return base_frequency*(pow(POWER_BASE, (note - BASE_NOTE)/SEMITONES));
}

#endif //PD_MIDI_H
