//
// Created by Admin on 12.06.2019.
//

#include "Master.h"

bool AMGMasterTrack::ARender(float *audioData, int numFrames) {

    double end_beat = beat;

    if (isPlaying) {
        ableton::Link::SessionState state = link.captureAppSessionState();
        bpm = state.tempo();
        std::chrono::microseconds time = link.clock().micros();
        std::chrono::microseconds second{1000000};
        std::chrono::microseconds lag = std::chrono::duration_cast<std::chrono::microseconds> ((second * numFrames) / sample_rate);
        phase = state.phaseAtTime(time + lag, size_denominator);
        beat = state.beatAtTime(time + lag, size_denominator);
        linkButton->progress(phase/size_denominator);
    }

    double increment = bpm / 60.0 / sample_rate;// + (beat - end_beat)/numFrames;

//    MData cmd;
//    cmd.status = 0x80;
//    cmd.data1 = 50;
//    cmd.data2 = 100;
//    MOut(cmd);

    for (int i = 0; i < numFrames; i++) {
        audioData[2*i] = 0;
        audioData[2*i+1] = 0;

        MRender(beat);

        if (*metronome_button) {
            if ((int)phase > (int)last_phase) metronome->tic(beat);
            else if ((int)phase < (int)last_phase) metronome->tac(beat);
            last_phase = phase;
            metronome->ARender(beat, &audioData[2*i], &audioData[2*i + 1]);
        }

        for (auto const& track : Tracks) {
            float l = 0, r = 0;
            track->ARender(beat, &l, &r);
            audioData[2*i] += l;
            audioData[2*i + 1] += r;
        }
        AEffects.ARender(beat, &audioData[2*i], &audioData[2*i + 1]);
        beat += increment;
        phase = fmod(phase + increment, size_denominator);

        audioData[2*i] *= 0.5;
        audioData[2*i + 1] *= 0.5;

//        masterWaveform->ARender(beat, &audioData[2*i], &audioData[2*i + 1]);
    }

    return true;
}