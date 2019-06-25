//
// Created by Admin on 13.06.2019.
//

#include "Arpeggiator.h"

Arpeggiator::Arpeggiator() : Arpeggiator(1/4.0) {}

Arpeggiator::Arpeggiator(double scale_)
{
    scale = scale_;
    last_played_note = 0;
    cycles = 0;
}

void Arpeggiator::apply(std::priority_queue<MidiData> *midiQueue, double beat)
{
    while (!midiQueue->empty()){
        MidiData md = midiQueue->top();
        if ((md.status & 0xF0) == NOTEON_HEADER) {
            if (md.data2) notes.insert({md.data1, md});
            else notes.erase(md.data1);
        }
        midiQueue->pop();
    }

    if (!notes.empty() && (int)(beat / scale) > cycles){
        auto i = notes.upper_bound(last_played_note);
        if (i == notes.end()) i = notes.begin();
        MidiData md = i->second;
        md.beat = beat;
        midiQueue->push(md);
        last_played_note = md.data1;
        cycles = (int)(beat / scale);
    }
}