//
// Created by Admin on 12.06.2019.
//

#ifndef PD_AUDIOEFFECT_H
#define PD_AUDIOEFFECT_H

#include <GUI/Button.h>
#include "GUI/Canvas.h"

class AudioEffect : public AMGCanvas{

    const float onoff_button_height = 0.08;
    const float onoff_button_ratio = 3.2;

public:

    Button * isOn;

    AudioEffect(){
        GAttachTexture("Textures/effect_canvas.bmp");
        isOn = new Button(L"ON/OFF", [](bool state){});
        isOn->setHeight(onoff_button_height);
        isOn->setRatio(onoff_button_ratio);
        GAttach(isOn);

        *isOn = true;
    }

    void GInit() override
    {
        isOn->place(0.98 - onoff_button_height*onoff_button_ratio/ratio, 0);
    }
};


#endif //PD_AUDIOEFFECT_H
