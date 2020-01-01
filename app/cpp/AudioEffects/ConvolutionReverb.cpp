//
// Created by ibelikov on 15.12.19.
//

#include "ConvolutionReverb.h"
#include <future>

ConvolutionReverb::ConvolutionReverb(const char * ir_file) : AudioEffect("Convolution reverb"){
    GSetRatio(2);

    ir.load(ir_file);

    active_buffer = 0;
    sample_counter = 0;

    ir_vis = new GUI::Plot<GUI::TimeGraph>(200);
    ir_vis->GPlace({0.35, 0.05});
    ir_vis->GSetHeight(0.9);
    ir_vis->GSetWidth(0.625);
    GAttach(ir_vis);

    low = new GUI::Slider("low", 0, -1, 1);
    low->GPlace({0.125, 0.05});
    low->GSetHeight(0.6);
    low->GSetWidth(0.05);
    GAttach(low);

    mid = new GUI::Slider("mid", 0, -1, 1);
    mid->GPlace({0.2, 0.05});
    mid->GSetHeight(0.6);
    mid->GSetWidth(0.05);
    GAttach(mid);

    high = new GUI::Slider("high", 0, -1, 1);
    high->GPlace({0.275, 0.05});
    high->GSetHeight(0.6);
    high->GSetWidth(0.05);
    GAttach(high);

    int stride = ir.getNumSamplesPerChannel()/200;
    float min = 0, max = 0;
    int point = 0;
    for (int i = 1; i < ir.getNumSamplesPerChannel() - 1; i ++){
        if (i % stride == 0) {
            if (point)
                ir_vis->graph->update(min);
            else
                ir_vis->graph->update(max);
            point = 1 - point;
        }
        if ((ir.samples[0][i + 1] > ir.samples[0][i]) && (ir.samples[0][i] < ir.samples[0][i-1]))
            min = ir.samples[0][i];
        if ((ir.samples[0][i + 1] < ir.samples[0][i]) && (ir.samples[0][i] > ir.samples[0][i-1]))
            max = ir.samples[0][i];
    }

    convolver.init(blockSize, &ir.samples[0][0], ir.samples[0].size()/5);

    eq.lg = 0.5;  // Boost bass by 50%
    eq.mg = 1.75; // Cut mid by 25%
    eq.hg = 2.0;  // Leave high band alone

    for (int i = 0; i < blockSize; i++){
        inBuf[0].push_back(0);
        inBuf[1].push_back(0);
        outBuf[0].push_back(0);
        outBuf[1].push_back(0);
        inBufClean[0].push_back(0);
        inBufClean[1].push_back(0);
    }

    drywet = new GUI::Encoder("dry/wet", 1);
    drywet->GPlace({0.01, 0.05});
    drywet->GSetHeight(0.25);
    GAttach(drywet);
    MConnect(drywet);

    low_freq = new GUI::Encoder("low", 1, 0, 800);
    low_freq->GPlace({0.125, 0.7});
    low_freq->GSetHeight(0.25);
    GAttach(low_freq);
    MConnect(low_freq);

    high_freq = new GUI::Encoder("high", 1, 1000, sample_rate/4);
    high_freq->GPlace({0.232, 0.7});
    high_freq->GSetHeight(0.25);
    GAttach(high_freq);
    MConnect(high_freq);

}

bool ConvolutionReverb::ARender(double beat, float *lsample, float *rsample){
    float sample = (*lsample + *rsample)/2;

    eq.lg = 1 + *low;
    eq.mg = 1 + *mid;
    eq.hg = 1 + *high;

    if ((*low_freq != last_low_freq) || (*high_freq != last_high_freq)) {
        init_3band_state(&eq, *low_freq, *high_freq, sample_rate);
        last_high_freq = *high_freq;
        last_low_freq = *low_freq;
    }

    float input = sample;
    float diff = sample - do_3band(&eq, sample);
    sample += diff;

    if (enabled()) {

        if (sample_counter == 0) {
            std::async([this](){
                convolver.process(&inBuf[active_buffer][0], &outBuf[active_buffer][0], blockSize);
            });
            active_buffer = 1 - active_buffer;
        }

        inBuf[active_buffer][sample_counter] = sample * 0.1f;

        sample = outBuf[active_buffer][sample_counter];
    }

    sample_counter = (sample_counter + 1) % blockSize;

    float prop = (*drywet + 1)/2;

    float l = input, r = input;

    *lsample = sample*prop + inBufClean[0][sample_counter] * (1-prop);
    *rsample = sample*prop + inBufClean[1][sample_counter] * (1-prop);

    inBufClean[0][sample_counter] = l;
    inBufClean[1][sample_counter] = r;

    return enabled();
}