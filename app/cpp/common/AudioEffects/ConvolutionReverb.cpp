//
// Created by ibelikov on 15.12.19.
//

#include "ConvolutionReverb.h"

ConvolutionReverb::ConvolutionReverb(const char * ir_file) : AudioEffect("Convolution reverb"){
    shape->setRatio(2);

    ir.load(ir_file);

    convolver.init(blockSize, &ir.samples[0][0], ir.samples.size());

    drywet = new GUI::Encoder("dry/wet", -1, [this](float value) {}, 3);
    drywet->shape->lPlace({0.05, 0.05});
    drywet->shape->lSetHeight(0.25);
    GAttach(drywet);
    MConnect(drywet);

}

void ConvolutionReverb::processBuffer() {
//    const size_t blockSize = blockSizeMin + (static_cast<size_t>(rand()) % (1+(blockSizeMax-blockSizeMin)));
//
//    const size_t remainingOut = out.size() - processedOut;
//    const size_t remainingIn = in.size() - processedIn;
//
//    const size_t processingOut = std::min(remainingOut, blockSize);
//    const size_t processingIn = std::min(remainingIn, blockSize);
//
//    memset(&inBuf[0], 0, inBuf.size() * sizeof(fftconvolver::Sample));
//    if (processingIn > 0)
//    {
//        memcpy(&inBuf[0], &in[processedIn], processingIn * sizeof(fftconvolver::Sample));
//    }
//
//    convolver.process(&inBuf[0], &out[processedOut], processingOut);
//
//    processedOut += processingOut;
//    processedIn += processingIn;
}

bool ConvolutionReverb::ARender(double beat, float *lsample, float *rsample){
    float sample = (*lsample + *rsample)/2;

    if (enabled()) {

        if (sample_counter < blockSize) {
            pushBuffer(sample);
            sample_counter ++;
        } else {
            sample_counter = 0;
            getResult();
            processBuffer();
            swapBuffers();
        }

    }

    float prop = (*drywet + 1)/2;

    *lsample = sample*prop + *lsample * (1-prop);
    *rsample = sample*prop + *rsample * (1-prop);
    return enabled();
}