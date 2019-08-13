//
// Created by Admin on 06.07.2019.
//

#include "Button.h"
#include "Text.h"

Button::Button(wchar_t * label, std::function<void(bool)> callback_){
    callback = callback_;
    state = false;
    GAttachShaders("Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainRect.fsh");
    GAttachTexture("Textures/button.bmp");

    auto txt = new Text("Fonts/Roboto-Regular.ttf", label);
    txt->place(0.07, 0.1, 0.7, 0.9);
    GAttach(txt);

    info_overlay.GAttachShaders("Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainColor.fsh");
    info_overlay.GSetColor(1, 0, 0, 0.2);
    info_overlay.place(0, 0, 1, 1);
    GAttach(&info_overlay);
}

void Button::GTapEnd()
{
    state = !state;
    info_overlay.GSetVisible(state);
    callback(state);
}