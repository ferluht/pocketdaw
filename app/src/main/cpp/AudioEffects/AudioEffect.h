//
// Created by Admin on 12.06.2019.
//

#ifndef PD_AUDIOEFFECT_H
#define PD_AUDIOEFFECT_H

#include "GUI/Canvas.h"

class AudioEffect : public AMGCanvas{

public:

    bool isOn;

    AudioEffect(){
        GAttachTexture("Textures/effect_canvas.bmp");
        isOn = true;
    }

    virtual void ARender(double beat, float * lsample, float * rsample){ }
};


#endif //PD_AUDIOEFFECT_H
