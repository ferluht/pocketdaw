//
// Created by ibelikov on 15.12.19.
//

#ifndef PD_CONVOLUTIONREVERB_H
#define PD_CONVOLUTIONREVERB_H


#include "AudioEffect.h"
#include <GUI/Encoder.h>

class ConvolutionReverb : public AudioEffect{

    GUI::Encoder * drywet;

public:

    ConvolutionReverb();

    bool ARender(double beat, float * lsample, float * rsample) override;
};


#endif //PD_CONVOLUTIONREVERB_H
