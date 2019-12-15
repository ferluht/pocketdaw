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

class ConvolutionReverb : public AudioEffect{

    const size_t blockSize = 256;

    GUI::Encoder * drywet;

    int sample_counter = 0;

    AudioFile<float> ir;

    std::vector<fftconvolver::Sample> inBuf;

    fftconvolver::FFTConvolver convolver;

    void processBuffer();

    inline void pushBuffer(float sample) {

    }

    void swapBuffers();
    void getResult();

public:

    ConvolutionReverb(const char * ir);

    bool ARender(double beat, float * lsample, float * rsample) override;
};


#endif //PD_CONVOLUTIONREVERB_H
