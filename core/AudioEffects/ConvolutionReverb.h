//
// Created by ibelikov on 15.12.19.
//

#ifndef PD_CONVOLUTIONREVERB_H
#define PD_CONVOLUTIONREVERB_H


#include "AudioEffect.h"
#include <GUI/Encoder.h>
#include <Utils/AudioFile.h>
#include <AudioEffects/FFTConvolver/Utilities.h>
#include <AudioEffects/FFTConvolver/FFTConvolver.h>
#include <AudioEffects/FFTConvolver/TwoStageFFTConvolver.h>
#include <GUI/Plot.h>
#include <GUI/Slider.h>
#include "ThreeBandEQ.h"

class ConvolutionReverb : public AudioEffect{

    const size_t blockSize = 1024;

    GUI::Encoder * drywet;

    int sample_counter = 0;

    AudioFile<float> ir;

    int active_buffer;

    EQSTATE eq;

    GUI::Slider * low;
    GUI::Slider * mid;
    GUI::Slider * high;

    GUI::Encoder * low_freq;
    GUI::Encoder * high_freq;

    float last_low_freq = 0;
    float last_high_freq = 0;

    std::vector<fftconvolver::Sample> inBuf[2];
    std::vector<fftconvolver::Sample> outBuf[2];
    std::vector<fftconvolver::Sample> inBufClean[2];

    GUI::Plot<GUI::TimeGraph> * ir_vis;

    fftconvolver::FFTConvolver convolver;

public:

    ConvolutionReverb(const char * ir);

    bool ARender(double beat, float * lsample, float * rsample) override;
};


#endif //PD_CONVOLUTIONREVERB_H
