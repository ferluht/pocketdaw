//
// Created by Admin on 06.07.2019.
//

#include "Button.h"
#include "Text.h"

Button::Button(wchar_t * label, float x, float y, float h, float w, const char * texture, std::function<void(bool)> callback_)
: Canvas(x, y, h, w, texture, true){
    callback = callback_;
    state = false;
    attach(new Text("Fonts/Roboto-Regular.ttf", label, 0.05, 0.05, 1, 0.9));
}

void Button::tapEnd()
{
    draw();
    state = !state;
    callback(state);
}