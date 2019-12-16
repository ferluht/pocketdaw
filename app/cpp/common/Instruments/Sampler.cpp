//
// Created by Admin on 18.09.2019.
//

#include "Sampler.h"

Sampler::Sampler(const char * sample_name_) : Instrument<SamplerState>(1, "Sampler"){
    shape->setRatio(1);
    sample_name = sample_name_;

    setConstPitch(true);

    sample.load(sample_name);

    graph = new GUI::TimeGraph(200);
    graph->shape->lPlace({0.01, 0.01});
    graph->shape->lSetHeight(0.7);
    graph->shape->lSetWidth(0.98);
    GAttach(graph);

    pitch = new GUI::Encoder("pitch", 0, 2, -12, 12);
    pitch->shape->lPlace({0.1, 0.73});
    pitch->shape->lSetHeight(0.25);
    GAttach(pitch);
    MConnect(pitch);

    base_frequency = sample_rate / 2 / M_PI;

    int stride = sample.getNumSamplesPerChannel()/200;
    float min = 0, max = 0;
    int point = 0;
    for (int i = 1; i < sample.getNumSamplesPerChannel() - 1; i ++){
        if (i % stride == 0) {
            if (point)
                graph->update(min);
            else
                graph->update(max);
            point = 1 - point;
        }
        if ((sample.samples[0][i + 1] > sample.samples[0][i]) && (sample.samples[0][i] < sample.samples[0][i-1]))
            min = sample.samples[0][i];
        if ((sample.samples[0][i + 1] < sample.samples[0][i]) && (sample.samples[0][i] > sample.samples[0][i-1]))
            max = sample.samples[0][i];
    }

    trig = new GUI::TapButton("trig", [this] (bool state) {triggered = true;});
    trig->shape->lPlace({0.75, 0.75});
    trig->shape->lSetHeight(0.2);
    trig->shape->lSetWidth(0.2);
    GAttach(trig);
    MConnect(trig);
}

void Sampler::MRender(double beat) {
    if (triggered) {
        MIn({beat, NOTEON_HEADER, 62, 100});
        triggered = false;
    }
}

void Sampler::IUpdateState(SamplerState *state, MData md) {
    if (((md.status & 0xF0) == NOTEON_HEADER) && (md.data2 != 0)) {
        if (const_pitch) state->note = base_note;
        else state->note = md.data1;
        state->volume = (float)md.data2/127.f;
        if (state->isActive()){
            state->transient = true;
            state->alpha = 0.001;
        }
        state->time = 0;
        state->setActive(true);
    }else{

    }
}

float Sampler::InterpolateCubic(float x0, float x1, float x2, float x3, float t)
{
    float a0, a1, a2, a3;
    a0 = x3 - x2 - x0 + x1;
    a1 = x0 - x1 - a0;
    a2 = x2 - x0;
    a3 = x1;
    return (a0 * (t * t * t)) + (a1 * (t * t)) + (a2 * t) + (a3);
}

float Sampler::InterpolateHermite4pt3oX(float x0, float x1, float x2, float x3, float t)
{
    float c0 = x1;
    float c1 = .5F * (x2 - x0);
    float c2 = x0 - (2.5F * x1) + (2 * x2) - (.5F * x3);
    float c3 = (.5F * (x3 - x0)) + (1.5F * (x1 - x2));
    return (((((c3 * t) + c2) * t) + c1) * t) + c0;
}

void Sampler::IARender(SamplerState *state, double beat, float *lsample, float *rsample) {

    if (state->time + 4 < sample.getNumSamplesPerChannel()) {

        float x = InterpolateHermite4pt3oX(
                sample.samples[0][(int) state->time],
                sample.samples[0][((int) state->time) + 1],
                sample.samples[0][((int) state->time) + 2],
                sample.samples[0][((int) state->time) + 3], state->time - (int) state->time) * state->volume;

        if (state->transient) {
            state->out = state->alpha * state->alpha * x + (1 - state->alpha * state->alpha) * state->out;
            state->alpha += 0.003;
        } else {

            if (state->time < 200) {
                x *= (state->time / 200) * (state->time / 200);
            }

            state->out = x;
        }

        if (state->alpha > 1) {
            state->transient = false;
        }

        *lsample += state->out;
        *rsample += state->out;
        state->time += getPhaseIncrement(state->note + *pitch);

    } else state->setActive(false);

}