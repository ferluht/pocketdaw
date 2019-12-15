//
// Created by ibelikov on 15.12.19.
//

#include "ConvolutionReverb.h"

ConvolutionReverb::ConvolutionReverb() : AudioEffect("Convolution reverb"){
    shape->setRatio(2);

    drywet = new GUI::Encoder("dry/wet", -1, [this](float value) {}, 3);
    drywet->shape->lPlace({0.25, 0.7});
    drywet->shape->lSetHeight(0.25);
    GAttach(drywet);
    MConnect(drywet);

}

bool ConvolutionReverb::ARender(double beat, float *lsample, float *rsample){
    float sample = (*lsample + *rsample)/2;

    if (enabled()) {

    }

    float prop = (*drywet + 1)/2;

    *lsample = sample*prop + *lsample * (1-prop);
    *rsample = sample*prop + *rsample * (1-prop);
    return enabled();
}