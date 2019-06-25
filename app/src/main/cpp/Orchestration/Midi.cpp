//
// Created by Admin on 12.06.2019.
//

#include <cmath>
#include "Midi.h"

bool operator<(const MidiData &lhs, const MidiData &rhs) {
    return lhs.beat > rhs.beat;
}


MidiData::MidiData(unsigned char status_, unsigned char data1_, unsigned char data2_) : MidiData(-1, status_, data1_, data2_) {}
    
MidiData::MidiData(double beat_, unsigned char status_, unsigned char data1_, unsigned char data2_)
{
    beat = beat_;
    status = status_;
    data1 = data1_;
    data2 = data2_;
}


MidiClip::MidiClip() : MidiClip(0) {}

MidiClip::MidiClip(double beat_begin) : MidiClip(beat_begin, 4) {}

MidiClip::MidiClip(double beat_begin, double pattern_length_)
{
    position = 0;
    begin = beat_begin;
    pattern_length = pattern_length_;
    end = 0;
}


void MidiClip::play(std::priority_queue<MidiData> *q, double beat)
{
    if (beat < end || end == 0){
        while(!Arrangement.empty() && (Arrangement[position].beat + loops*pattern_length < beat - begin)){
            q->push(Arrangement[position]);
            position ++;
            if (position == Arrangement.size()){
                position = 0;
                loops++;
            }
        }
    }
}

void MidiClip::insertNow(MidiData md)
{
    Arrangement.insert(Arrangement.begin() + position, md);
}

void MidiClip::insert(MidiData md, double beat)
{
    std::vector<MidiData>::iterator pos = Arrangement.begin();
    beat = fmod(beat - begin, pattern_length);
    md.beat = beat;
    while (Arrangement.size() && pos != Arrangement.end() && pos->beat < beat) pos ++;
    Arrangement.insert(pos, md);
}

void MidiClip::reset()
{
    position = 0;
}

MidiClip * createMetronomeMidi(){
    MidiClip * mc = new MidiClip;
    MidiData on(0x91, 40, 100);
    MidiData off(0x91, 40, 0);
    mc->insert(on, 0);
//    mc->insert(off, 3);

    on.data1 = 42;
    mc->insert(on, 0);
//    mc->insert(off, 3);

    on.data1 = 45;
    mc->insert(on, 0);
//    mc->insert(off, 3);

//    mc->insert(on, 1);
//    mc->insert(off, 1.4);
//
//    mc->insert(on, 2);
//    mc->insert(off, 2.4);
//
//    mc->insert(on, 3);
//    mc->insert(off, 3.4);

    return mc;
}