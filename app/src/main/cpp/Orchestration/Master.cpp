//
// Created by Admin on 12.06.2019.
//

#include "Master.h"
#include <Instruments/Metronome.h>
#include <GUI/Button.h>
#include <AudioEffects/Waveform.h>
#include <AudioEffects/Lissajous.h>
#include "AudioEffects/StereoDelay.h"

Master::Master() :
Canvas(0, 0, 1, 1, "Textures/background.bmp", false),
link(DEFAULT_BPM)
{
    link.enable(true);

    focusTrack = 0;

    auto * cue = new Track;
    auto metr = new Metronome;
    cue->initInstrument(metr);
    cue->setVisible(true);

    auto * cue2 = new Track;
    auto metr2 = new Metronome;
    cue2->initInstrument(metr2);
    cue2->setVisible(false);

    cue->addAudioEffect(new StereoDelay(0.7));


    addTrack(cue);
    addTrack(cue2);

//    auto graph = new SimpleGraph(100, 0.05, 0.05, 3, 0.9, 0.9);
//    attach(graph);

    //    attach(new Button(0.5, 0.5, 0.1, 0.1, "Textures/encoder.bmp", [cue](bool state){cue->addAudioEffect(new Delay(0.2, 0.09));}));
//    cue->addAudioEffect(new Delay(0.2, 0.09));
//    cue->addAudioEffect(new Delay(0.2, 0.09));
//    cue->addAudioEffect(new Delay(0.2, 0.09));
//    cue->addAudioEffect(new Delay(0.2, 0.09));
//    cue->addAudioEffect(new Delay(0.2, 0.09));
//    cue->addAudioEffect(new Delay(0.2, 0.09));
//    cue->addAudioEffect(new Delay(0.2, 0.09));
//    cue->addAudioEffect(new Delay(0.2, 0.09));
//    cue->addAudioEffect(new Delay(8000, 0.3));
//    cue->addAudioEffect(new Waveform(0.01, 0.5, 0.5));


//    attach(new Button(0,0,0,0, "Textures/container.bmp", []{}));
//    addChildObject(new Button("Textures/container.bmp", 0.9, 0.7));

//    addAudioEffect(new Delay(3300, 0.2));
//    addAudioEffect(new Delay(900, 0.4));
//    addAudioEffect(new Delay(17000, 0.2));
//    addAudioEffect(new Delay(10000, 0.3));
//    addAudioEffect(new Delay(8000, 0.6));

    addAudioEffect(new Waveform(200, 0, 0));
    addAudioEffect(new Lissajous(200, 0, 0));

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
        audioData[2*i] = 0;
        audioData[2*i+1] = 0;
        for (auto const& track : Tracks) {
            float l = 0, r = 0;
            track->render(beat, &l, &r);
            audioData[2*i] += l;
            audioData[2*i + 1] += r;
            beat += increment;
        }
        for (auto const& effect : AudioEffects) {
            effect->apply(&audioData[2*i], &audioData[2*i + 1]);
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
    track->place(0,0.5,0.5,1);
    Tracks.push_back(track);
    attach(track);
}

void Master::delTrack(int pos)
{
    Tracks.erase(Tracks.begin() + pos);
}

void Master::addAudioEffect(AudioEffect *effect)
{
    if (AudioEffects.empty()) {
        effect->place(0.01f, 0.01f, 0.48, 0.3);
    } else {
        effect->place(AudioEffects.back()->x + AudioEffects.back()->width + 0.01f, AudioEffects.back()->y, 0.48, 0.3);
    }

    AudioEffects.push_back(effect);
    attach(effect);
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

    if (md.status == 0xb0){
        switch (md.data1){
            case 0x15:
                Tracks[focusTrack]->setVisible(false);
                if (focusTrack < Tracks.size() - 1) focusTrack ++;
                Tracks[focusTrack]->setVisible(true);
                break;
            case 0x16:
                Tracks[focusTrack]->setVisible(false);
                if (focusTrack > 0) focusTrack --;
                Tracks[focusTrack]->setVisible(true);
                break;
            default:
                break;
        }
    }

    md.beat = beat;
    Tracks[focusTrack]->MidiQueue.push(md);
}