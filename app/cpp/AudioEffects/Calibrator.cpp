//
// Created by ferluht on 13/09/2020.
//

#include "Calibrator.h"

Calibrator::Calibrator() : AudioEffect("Calibrator")
{
    GSetRatio(0.45f);

    en = new GUI::Button("calibrate", [this](bool state){
        deltatime = initial_deltatime;
        for (int i = 0; i < window_size; i ++){
            window[i] = 0;
        }
    });
    en->GPlace({0.05, 0.05});
    en->GSetHeight(0.17);
    en->GSetWidth(0.9);
    GAttach(en);
    MConnect(en);

    lag = new GUI::Text("lag: --", GREEN, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    lag->GPlace({0.05, 0.35});
    lag->GSetHeight(0.1);
    lag->GSetWidth(0.9);
    GAttach(lag);

    window = new float[window_size];
}

void Calibrator::meanstd() {
    mean = 0;
    for (int i = 0; i < window_size; i++) {
        mean += window[i];
    }
    mean /= window_size;
    float sqrd = 0;
    for (int i = 0; i < window_size; i++) {
        sqrd += std::pow(window[i] - mean, 2);
    }
    std = std::sqrt(1.0f/(window_size - 1) * sqrd);
}

bool Calibrator::ARender(double beat, float *lsample, float *rsample){

    if (enabled()) {
        *lsample *= 0.2;
        *rsample *= 0.2;
    }

    if (*en) {

        lag_counter ++;

        meanstd();
        bool peak_detected = false;
        if (*lsample > (mean + 10*std)) peak_detected = true;

        if (peak_detected) {
            AObject::latency = lag_counter;
            char l[100];
            sprintf(l, "lag: %d ms", (int)(AObject::latency / AObject::sample_rate * 1000.f));
            lag->TSetText(l);
            *en = false;
        }

        window[wpos] = *lsample;
        wpos = (wpos + 1) % window_size;

        *lsample = 0;
        *rsample = 0;

        if (deltatime > 0) {
            *lsample = 1;
            *rsample = 1;
            deltatime--;
            lag_counter = initial_deltatime;
        }

//        time_since_last_spike --;
//        if (peak_detected || (time_since_last_spike == 0)) {
//            deltatime = initial_deltatime;
//            period = initial_period - time_since_last_spike;
//            time_since_last_spike = period;
//        }
    }

    return enabled();
}