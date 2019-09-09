//
// Created by Admin on 06.07.2019.
//

#include "Button.h"

Button::Button(wchar_t * label, std::function<void(bool)> callback_){
    callback = callback_;
    state = false;
    GAttachShaders("Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainRect.fsh");
    GAttachTexture("Textures/button.bmp");

    text = new Text("Fonts/Roboto-Regular.ttf", label);
    text->setHeight(textheight);
    GAttach(text);

    info_overlay.GSetColor(1, 0, 0, 0.2);
    info_overlay.GSetVisible(state);
}

void Button::GInit()
{
    text->place(0.07 + (0.86 - text->ratio*textheight/ratio)/2 , (1 - textheight)/2);
}

GObject * Button::GTapEnd(const ndk_helper::Vec2& v)
{
    state = !state;
    info_overlay.GSetVisible(state);
    callback(state);
    return nullptr;
}