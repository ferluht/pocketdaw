//
// Created by Admin on 12.06.2019.
//

#include "Master.h"
#include "../Instruments/Metronome.h"
#include "../AudioEffects/Delay.h"

Master::Master() : link(DEFAULT_BPM)
{
    link.enable(true);
    auto * cue = new Track;
    cue->TrackInstrument = new Metronome;
    auto * delay = new Delay(2000, 0.3);
//    cue->addAudioEffect(delay);
    addTrack(cue);

//    addAudioEffect(new Delay(3300, 0.2));
//    addAudioEffect(new Delay(900, 0.4));
//    addAudioEffect(new Delay(17000, 0.2));
//    addAudioEffect(new Delay(10000, 0.3));
//    addAudioEffect(new Delay(8000, 0.6));

    size_denominator = 4;
    stopPressed_ = 0;
    isPlaying = false;
    beat = -1;
    phase = -1;
}

void Master::render(float *audioData, int32_t numFrames) {

    if (isPlaying) {
        ableton::Link::SessionState state = link.captureAppSessionState();
        bpm = state.tempo();
        std::chrono::microseconds time = link.clock().micros();
        phase = state.phaseAtTime(time, size_denominator);
        beat = state.beatAtTime(time, size_denominator);
    }

    double increment = bpm / 60.0 / sample_rate * numFrames;

    for (int i = 0; i < numFrames; i++) {
        audioData[i] = 0;
        for (auto const& track : Tracks) {
            audioData[i] += track->render(beat);
            beat += increment;
        }
        for (auto const& effect : AudioEffects) {
            audioData[i] = effect->apply(audioData[i]);
        }
    }
}

void Master::start()
{
    isPlaying = true;
    stopPressed_ = 0;
    beat = -1;
    phase = -1;
}

void Master::stop()
{
    isPlaying = false;
    if(stopPressed_ ++ > 1) beat = -1, phase = -1;
}

void Master::addTrack(Track *track)
{
    Tracks.push_back(track);
}

void Master::delTrack(int pos)
{
    Tracks.erase(Tracks.begin() + pos);
}

void Master::addAudioEffect(AudioEffect *effect)
{
    AudioEffects.push_back(effect);
}

void Master::delAudioEffect(int pos)
{
    AudioEffects.erase(AudioEffects.begin() + pos);
}

void Master::setSampleRate(int samplerate)
{
    sample_rate = samplerate;
}

void Master::receiveMIDI(MidiData md)
{
    ableton::Link::SessionState state = link.captureAppSessionState();
    bpm = state.tempo();
    std::chrono::microseconds time = link.clock().micros();
    phase = state.phaseAtTime(time, size_denominator);
    beat = state.beatAtTime(time, size_denominator);

    md.beat = beat;
    Tracks[md.status & 0x03 - 1]->MidiQueue.push(md);
}