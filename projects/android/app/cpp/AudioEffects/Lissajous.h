//
// Created by Admin on 30.07.2019.
//

#ifndef PD_LISSAJOUS_H
#define PD_LISSAJOUS_H


#include "AudioEffect.h"
#include "../GUI/Graph.h"

class Lissajous : public AudioEffect{
public:

    int r = 1;
    float window = 3;
    float accumulator = 0;
    int ai = 0;

    XYGraph * graph;

    Lissajous(float n, float x_, float y_);

    void apply(float * lsample, float * rsample) override ;
};


#endif //PD_LISSAJOUS_H
