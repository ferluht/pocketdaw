//
// Created by ibelikov on 11.07.19.
//

#ifndef PD_WAVEFORM_H
#define PD_WAVEFORM_H

#include "../GUI/GraphicObject.h"
#include "AudioEffect.h"

#define BUFFER_SIZE 2000

class Waveform : public AudioEffect{
public:

    float buffer[BUFFER_SIZE];

    int r = 1;
    float window = 3;
    float accumulator = 0, prev_acc = 0;
    int ai = 0;

    GLuint vertexbuffer;

    Waveform(float n, float x, float y);

    float apply(float sample) override ;

    void SetPosition(float x, float y);
    void SetSize(float x, float y);
    void grender(float dTime) override ;
    void dragHandler(ndk_helper::Vec2 v) override ;
    void dragBegin(ndk_helper::Vec2 v, float xscale, float yscale) override ;
    void dragEnd() override ;
};


#endif //PD_WAVEFORM_H
