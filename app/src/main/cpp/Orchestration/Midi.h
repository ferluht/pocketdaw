//
// Created by Admin on 12.06.2019.
//

#ifndef PD_MIDI_H
#define PD_MIDI_H

#include <stdlib.h>
#include <vector>
#include <queue>
#include <cmath>

#define NOTEOFF_HEADER 128
#define NOTEON_HEADER 144
#define POLYPRESSURE_HEADER 160
#define CC_HEADER 176
#define PITCHWHEEL_HEADER 224

#define CC_MODWHEEL 1
#define CC_BREATH_CONTROLLER 2
#define CC_AFTERTOUCH 3
#define CC_FOOT_CONTROLLER 4
#define CC_PORTAMENTO_TIME 5
#define CC_DATA_ENTRY 6
#define CC_MAIN_VOLUME 7
#define CC_DAMPER_PEDAL 64
#define CC_PORTAMENTO 65
#define CC_SOSTENUTO 66
#define CC_SOFT_PEDAL 67
#define CC_CHORUS 93


#define BASE_NOTE 61.0
#define POWER_BASE 2.0
#define SEMITONES 24.0

class MidiData {

public:

    double beat;
    unsigned char status;
    unsigned char data1;
    unsigned char data2;
    MidiData(unsigned char status_, unsigned char data1_, unsigned char data2_);
    MidiData(double beat_, unsigned char status_, unsigned char data1_, unsigned char data2_);
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
