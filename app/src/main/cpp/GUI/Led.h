//
// Created by Admin on 04.09.2019.
//

#ifndef PD_LED_H
#define PD_LED_H

#include "Canvas.h"

class Led : public GCanvas{
public:

    bool state;
    unsigned char counter;

    Led(bool state_) {
        GAttachShaders("Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainColor.fsh");
        setRatio(1);
        counter = 0;
        state = state_;
        GSetColor(1, 0.8, 0, 1*state_);
    }

    inline void on(){
        state = true;
        GSetColor(1, 0.8, 0, 1);
    }

    inline void off(){
        state = false;
        GSetColor(1, 0.8, 0, 0);
    }

    inline void toggle(){
        if (state) off();
        else on();
    }

    inline void toggle(int divider){
        counter ++;
        if (counter >= divider) counter = 0;
        if (counter) return;
        if (state) off();
        else on();
    }

    operator bool() const { return state; }
};


#endif //PD_LED_H
