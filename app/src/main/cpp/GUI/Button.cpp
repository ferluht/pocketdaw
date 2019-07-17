//
// Created by Admin on 06.07.2019.
//

#include "Button.h"

Button::Button(float x, float y, float h, float w, const char * texture, std::function<void(bool)> callback_)
: Canvas(x, y, h, w, texture){
    callback = callback_;
    state = false;
}

void Button::tapEnd()
{
    draw();
    state = !state;
    callback(state);
}