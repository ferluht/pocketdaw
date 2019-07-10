//
// Created by Admin on 13.06.2019.
//

#include "Delay.h"
#include "../GUI/Encoder.h"

Delay::Delay(float delayTime_, float feedback_) : AudioEffect()
{

    relativePosition.x = 0.3;
    relativePosition.y = 0.3;
    relativePosition.width = 0.4;
    relativePosition.height = 0.2;
    addChildObject(new Encoder("Textures/encoder.bmp", 0.6f, 0.2f, &feedback));
    addChildObject(new Encoder("Textures/encoder.bmp", 0.1f, 0.2f, &delayTime));

    for (int i = 0; i < 50000; i ++){
        buffer[i] = 0;
    }
    delayTime = delayTime_;
    feedback = feedback_;
}

float Delay::apply(float sample)
{
    int index = (int)(delayTime*50000) - position;
    sample += buffer[index];
    buffer[index] = sample*feedback;
    position = (position + 1)%((int)(delayTime*50000));
    return sample;
}