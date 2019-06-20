//
// Created by Admin on 12.06.2019.
//

#include "Metronome.h"
#include <cmath>

Metronome::Metronome() : Instrument()
{
    duration = 0;
}

void Metronome::midiCommand(MidiData md)
{
    if (md.vel) {
        phase = 0;
        phase_inc = M_PI / 48000.0 * getFrequency(440.0, md.note);
        vol = md.vel / 127.0;
        ad.attack();
    } else {
        ad.release();
    }
}

float Metronome::render()
{
    float sample = sin(phase);
    sample = ad.apply(sample) * vol;
    phase += phase_inc;
    return sample;
}