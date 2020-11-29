//
// Created by ferluht on 29/11/2020.
//

#ifndef ANDROID_PITCHTRACKER_H
#define ANDROID_PITCHTRACKER_H


#include <GUI/Graph.h>
#include <GUI/Plot.h>
#include "AudioEffect.h"

class PitchTracker : public AudioEffect {

    float minfreq, maxfreq;

    std::thread * worker_thread;
    std::mutex run_lock;
    std::mutex stop_lock;
    std::mutex end_lock;
    std::mutex destroy_lock;

    float * a_buffer;
    int a_buffer_pos = 0;
    float * p_buffer;
    float * autocorrelation;
    float p_freq, p_amp;

    int buffer_size;

    GUI::Plot<GUI::TimeGraph> * plot;

public:

    float freq, amp;

    PitchTracker(float minfreq_, float maxfreq_) : AudioEffect("Pitch Tracker") {
        minfreq = minfreq_;
        maxfreq = maxfreq_;

        buffer_size = (int)(1.5f * AObject::sample_rate / minfreq);
        a_buffer_pos = 0;

        a_buffer = new float [buffer_size];
        p_buffer = new float [buffer_size];
        autocorrelation = new float [buffer_size];

        GSetRatio(0.6f);

        plot = new GUI::Plot<GUI::TimeGraph>(buffer_size);
        plot->GPlace({0.01, 0.01});
        plot->GSetHeight(0.98*0.5);
        plot->GSetWidth(0.98);
        GAttach(plot);

        run_lock.lock();
        stop_lock.unlock();
        end_lock.unlock();
        destroy_lock.lock();
        worker_thread = new std::thread(RunInThread, this);
    }

    ~PitchTracker(){
        destroy_lock.unlock();
    }

    bool ARender(double beat, float * lsample, float * rsample) override {
        a_buffer[a_buffer_pos] = *lsample;
        a_buffer_pos = (a_buffer_pos + 1) % buffer_size;
        if (a_buffer_pos == 0) {
            ThreadWait();
            float * t = a_buffer;
            a_buffer = p_buffer;
            p_buffer = t;
            freq = p_freq;
            amp = p_amp;
            ProcessInThread();
        }
        return true;
    }

    void ProcessInThread() {
        stop_lock.lock();

        stop_lock.unlock();
        end_lock.lock();
        run_lock.unlock();
    }

    void ThreadWait() {
        end_lock.lock();
        end_lock.unlock();
    }

    static void RunInThread(PitchTracker * tracker) {
        while(!tracker->destroy_lock.try_lock()) {
            tracker->run_lock.lock();
            tracker->stop_lock.lock();

            for (int i = 0; i < tracker->buffer_size; i++) {
                tracker->autocorrelation[i] = 0;
                for (int j = 0; j < tracker->buffer_size; j++) {
                    tracker->autocorrelation[i] += tracker->p_buffer[j] * tracker->p_buffer[(j + i) % tracker->buffer_size];
                }
//                tracker->plot->update(tracker->autocorrelation[i]);
            }

            int argmax = (int)(AObject::sample_rate / tracker->maxfreq);

            float min = 0, max = 0;

            for (int i = argmax; i < tracker->buffer_size / 2; i++) {
                if (abs(tracker->autocorrelation[i]) > abs(tracker->autocorrelation[argmax])) argmax = i;
                if (tracker->p_buffer[i] < min) min = tracker->p_buffer[i];
                if (tracker->p_buffer[i] > max) max = tracker->p_buffer[i];
            }

            tracker->p_amp = max - min;

            tracker->p_freq = AObject::sample_rate / (float)argmax;

            tracker->plot->update(tracker->p_freq / 1000);

            tracker->stop_lock.unlock();
            tracker->end_lock.unlock();
        }
    }
};


#endif //ANDROID_PITCHTRACKER_H
