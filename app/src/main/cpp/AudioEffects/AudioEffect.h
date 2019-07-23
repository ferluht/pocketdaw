//
// Created by Admin on 12.06.2019.
//

#ifndef PD_AUDIOEFFECT_H
#define PD_AUDIOEFFECT_H

#include "GUI/GUI.h"

class AudioEffect : public Canvas{

public:

    AudioEffect(float x, float y, float h, float w)
    : Canvas(x, y, h, w, "Textures/effect_canvas.bmp") {};

    virtual float apply(float sample)
    {
        return sample;
    }
};


#endif //PD_AUDIOEFFECT_H
