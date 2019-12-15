//
// Created by ibelikov on 15.12.19.
//

#include "ConvolutionReverb.h"
#include <future>

ConvolutionReverb::ConvolutionReverb(const char * ir_file) : AudioEffect("Convolution reverb"){
    shape->setRatio(2);

    ir.load(ir_file);

    active_buffer = 0;

    convolver.init(blockSize, &ir.samples[0][0], ir.samples.size());

    for (int i = 0; i < blockSize; i++){
        inBuf[0].push_back(0);
        inBuf[1].push_back(0);
        outBuf[0].push_back(0);
        outBuf[1].push_back(0);
    }

    drywet = new GUI::Encoder("dry/wet", -1, [this](float value) {}, 3);
    drywet->shape->lPlace({0.05, 0.05});
    drywet->shape->lSetHeight(0.25);
    GAttach(drywet);
    MConnect(drywet);

}

bool ConvolutionReverb::ARender(double beat, float *lsample, float *rsample){
    float sample = (*lsample + *rsample)/2;

    if (enabled()) {

        if (sample_counter == 0) {
//            std::async([this](){
            convolver.process(&inBuf[active_buffer][0], &outBuf[active_buffer][0], blockSize);
//            });
            active_buffer = 1 - active_buffer;
        }

        inBuf[active_buffer][sample_counter] = sample;

        sample = outBuf[active_buffer][sample_counter];
        sample_counter = (sample_counter + 1) % blockSize;
    }

    float prop = (*drywet + 1)/2;

    *lsample = sample*prop + *lsample * (1-prop);
    *rsample = sample*prop + *rsample * (1-prop);
    return enabled();
}