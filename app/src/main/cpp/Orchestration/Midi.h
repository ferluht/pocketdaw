//
// Created by Admin on 12.06.2019.
//

#ifndef PD_MIDI_H
#define PD_MIDI_H

#include <stdlib.h>
#include <vector>
#include <queue>
#include <cmath>
#include "Basic/M.h"

class MidiClip {

public:

    double begin, pattern_length, loops;
    int end;

    unsigned int position;

    std::vector<MData> Arrangement;

    MidiClip();
    MidiClip(double beat_begin);
    MidiClip(double beat_begin, double pattern_length_);

    void play(std::priority_queue<MData> * q, double beat);
    void insertNow(MData md);
    void insert(MData md, double beat);
    void reset();
    void stop(double beat);
};


MidiClip * createMetronomeMidi();

#endif //PD_MIDI_H
