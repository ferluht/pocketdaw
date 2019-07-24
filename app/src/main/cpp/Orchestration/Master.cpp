//
// Created by Admin on 12.06.2019.
//

#include "Master.h"
#include <Instruments/Metronome.h>

Master::Master() :
Canvas(0, 0, -1, -1, "Textures/container.bmp"),
link(DEFAULT_BPM)
{
    link.enable(true);
    auto * cue = new Track;
    auto metr = new Metronome;
    cue->initInstrument(metr);
    cue->addAudioEffect(new Delay(8000, 0.3));
    cue->addAudioEffect(new Delay(8000, 0.3));
//    cue->addAudioEffect(new Waveform(0.01, 0.5, 0.5));
    addTrack(cue);

//    attach(new Button(0,0,0,0, "Textures/container.bmp", []{}));
//    addChildObject(new Button("Textures/container.bmp", 0.9, 0.7));

//    addAudioEffect(new Delay(3300, 0.2));
//    addAudioEffect(new Delay(900, 0.4));
//    addAudioEffect(new Delay(17000, 0.2));
//    addAudioEffect(new Delay(10000, 0.3));
//    addAudioEffect(new Delay(8000, 0.6));

    size_denominator = 4;
    stopPressed_ = 0;
    isPlaying = true;
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

//        audioData[i] = (audioData[i] + prev_sample)/2;
//        prev_sample = audioData[i];
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

//void Master::addGraphic(GraphicObject *gr)
//{
//    Graphics.push_back(gr);
//}
//
//void Master::delGraphic(int pos)
//{
//    Graphics.erase(Graphics.begin() + pos);
//}
//
void Master::addTrack(Track *track)
{
//    track->relativePosition.x = 0;
//    track->relativePosition.y = 0;
//    if (!Tracks.empty()) {
//        track->relativePosition.x =
//                Tracks.back()->relativePosition.x + Tracks.back()->relativePosition.width;
//        track->relativePosition.y = Tracks.back()->relativePosition.y;
//    }
//    track->relativePosition.width = 0.3;
//    track->relativePosition.height = 1;
    Tracks.push_back(track);
    attach(track);
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