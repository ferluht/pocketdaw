//
// Created by Admin on 06.07.2019.
//

#ifndef PD_BUTTON_H
#define PD_BUTTON_H

#include "Knob.h"

class Button : public Knob{
public:

    bool state;
    std::function<void(bool)> callback;

    Button(wchar_t * label, std::function<void(bool)> callback_);

    void GTapEnd() override ;

    operator bool() const { return state; }

    GObject * GFindFocusObject(const ndk_helper::Vec2& point) override {return this;}
};


#endif //PD_BUTTON_H
