//
// Created by Admin on 12.06.2019.
//

#ifndef PD_AUDIOEFFECT_H
#define PD_AUDIOEFFECT_H

#include "GUI/Canvas.h"

class AudioEffect : public Canvas{

public:

    bool isOn;

    AudioEffect(float x, float y, float h, float w)
    : Canvas(x, y, h, w, "Textures/effect_canvas.bmp", false) {isOn = true;};

    virtual void apply(float * lsample, float * rsample){ }
};


#endif //PD_AUDIOEFFECT_H
