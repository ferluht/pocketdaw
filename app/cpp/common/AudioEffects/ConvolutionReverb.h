//
// Created by ibelikov on 15.12.19.
//

#ifndef PD_CONVOLUTIONREVERB_H
#define PD_CONVOLUTIONREVERB_H


#include "AudioEffect.h"
#include <GUI/Encoder.h>
#include <common/Utils/AudioFile.h>
#include <common/AudioEffects/FFTConvolver/Utilities.h>
#include <common/AudioEffects/FFTConvolver/FFTConvolver.h>
#include <common/AudioEffects/FFTConvolver/TwoStageFFTConvolver.h>

class ConvolutionReverb : public AudioEffect{

    const size_t blockSize = 1024;

    GUI::Encoder * drywet;

    int sample_counter = 0;

    AudioFile<float> ir;

    int active_buffer;

    std::vector<fftconvolver::Sample> inBuf[2];
    std::vector<fftconvolver::Sample> outBuf[2];
    std::vector<fftconvolver::Sample> inBufClean[2];

    fftconvolver::FFTConvolver convolver;
    fftconvolver::TwoStageFFTConvolver tsconvolver;

public:

    ConvolutionReverb(const char * ir);

    bool ARender(double beat, float * lsample, float * rsample) override;
};


#endif //PD_CONVOLUTIONREVERB_H
