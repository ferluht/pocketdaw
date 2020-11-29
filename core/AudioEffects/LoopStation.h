//
// Created by ferluht on 27/11/2020.
//

#ifndef ANDROID_LOOPSTATION_H
#define ANDROID_LOOPSTATION_H


#include "AudioEffect.h"
#include "Looper.h"

class LoopStation : public AudioEffect {

    std::vector<Looper *> loopers;
    int n_loopers;

    Looper * master_looper;
    Looper * focus_looper;

    int active_loopers = 0;

public:

    LoopStation(int n_loopers_) : AudioEffect("Loop Station") {
        n_loopers = n_loopers_;
        GSetRatio(Looper::looper_ratio * n_loopers);

        master_looper = nullptr;

        for (int i = 0; i < n_loopers; i++) {
            Looper * looper = new Looper([this](Looper * l, Looper::LOOPER_EVENT event) {
                switch (event) {
                    case Looper::LOOPER_EVENT::RECORDING_START:
                        {
                            if (master_looper == nullptr) master_looper = l;
                        }
                    case Looper::LOOPER_EVENT::OVERDUB_START:
                        {
                            active_loopers ++;
                        } break;
                    case Looper::LOOPER_EVENT::RECORDING_END:
                    case Looper::LOOPER_EVENT::OVERDUB_END:
                        {
                            active_loopers --;
                        } break;
                    case Looper::LOOPER_EVENT::LOOP_END:
                        {
                            if (master_looper == l)
                                for (int j = 0; j < n_loopers; j++) loopers[j]->sync_tick();
                        } break;
                    case Looper::LOOPER_EVENT::CLEAR:
                        {
                            if (master_looper == l) master_looper = nullptr;
                        } break;
                    case Looper::LOOPER_EVENT::FOCUS:
                        {
                            focus_looper->setMonitoring(false);
                            focus_looper = l;
                            focus_looper->setMonitoring(true);
                        } break;
                    default:
                        break;
                }
            });
            looper->NoHeader();
            looper->GPlace({1.0f / n_loopers * i, 0});
            looper->GSetHeight(1.0f);
            GAttach(looper);
            loopers.push_back(looper);
        }

        focus_looper = loopers[0];
        focus_looper->setMonitoring(true);
    }

    bool ARender(double beat, float * lsample, float * rsample) override {

        float mix_l = 0, mix_r = 0;

        for (int i = 0; i < n_loopers; i++) {
            float l = *lsample, r = *rsample;
            loopers[i]->ARender(beat, &l, &r);
            mix_l += l;
            mix_r += r;
        }

        if (mix_l > 1) mix_l = 1;
        if (mix_l < -1) mix_l = -1;
        if (mix_r > 1) mix_r = 1;
        if (mix_r < -1) mix_r = -1;

        *lsample = mix_l;
        *rsample = mix_r;

        return true;
    }

};


#endif //ANDROID_LOOPSTATION_H
