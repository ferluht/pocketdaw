//
// Created by Admin on 12.06.2019.
//

#include "Master.h"

bool AMGMasterTrack::ARender(const float * inputData, int inputFrames, float * outputData, int outputFrames) {

    double end_beat = beat;

    if (isPlaying) {
        ableton::Link::SessionState state = link.captureAppSessionState();
        bpm = state.tempo();
        std::chrono::microseconds time = link.clock().micros();
        std::chrono::microseconds second{1000000};
        std::chrono::microseconds lag = std::chrono::duration_cast<std::chrono::microseconds> ((second * outputFrames) / sample_rate);
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

    track_lock.lock();

    float * samples[Tracks.size()];

    for (int i = 0; i < Tracks.size(); i ++) {
        samples[i] = new float[outputFrames * 2];
        Tracks[i]->AMGTrackProcess(beat, increment, inputData, inputFrames, samples[i], outputFrames);
    }

//    for (int i = 0; i < numFrames; i++) {
//        audioData[2 * i] = 0;
//        audioData[2 * i + 1] = 0;
//    }

    for (int i = 0; i < Tracks.size(); i ++) {
        Tracks[i]->AMGTrackWait();
        for (int j = 0; j < outputFrames; j++) {
            outputData[2*j] += samples[i][2*j];
            outputData[2*j + 1] += samples[i][2*j + 1];
        }
    }

    track_lock.unlock();

    for (int i = 0; i < outputFrames; i++) {

        if (*metronome_button) {
            if ((int)phase > (int)last_phase) metronome->tic(beat);
            else if ((int)phase < (int)last_phase) metronome->tac(beat);
            last_phase = phase;
            metronome->ARender(beat, &outputData[2*i], &outputData[2*i + 1]);
        }

        AEffects.ARender(beat, &outputData[2*i], &outputData[2*i + 1]);
        beat += increment;
        phase = fmod(phase + increment, size_denominator);

        outputData[2*i] *= 0.5;
        outputData[2*i + 1] *= 0.5;

//        masterWaveform->ARender(beat, &audioData[2*i], &audioData[2*i + 1]);
    }

    return true;
}