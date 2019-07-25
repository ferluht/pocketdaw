//
// Created by Admin on 06.07.2019.
//

#include "Button.h"
#include "Text.h"

Button::Button(float x, float y, float h, float w, const char * texture, std::function<void(bool)> callback_)
: Canvas(x, y, h, w, texture){
    callback = callback_;
    state = false;
    attach(new Text("Fonts/Roboto-Regular.ttf", L"button", 200, 200));
}

void Button::tapEnd()
{
    draw();
    state = !state;
    callback(state);
}