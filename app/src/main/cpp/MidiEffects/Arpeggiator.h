//
// Created by Admin on 13.06.2019.
//

#ifndef PD_ARPEGGIATOR_H
#define PD_ARPEGGIATOR_H

#include "MidiEffect.h"
#include <map>

#include <random>

class Arpeggiator : public MidiEffect{

public:

    double scale;

    int cycles;

    std::default_random_engine generator;

    std::map<unsigned char, MData> notes;
    unsigned char last_played_note;

    Arpeggiator();
    Arpeggiator(double scale_);

    void apply(std::priority_queue<MData> * midiQueue, double beat) override ;
};


#endif //PD_ARPEGGIATOR_H
