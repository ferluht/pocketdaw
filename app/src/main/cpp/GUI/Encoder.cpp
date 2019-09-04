//
// Created by Admin on 06.07.2019.
//

#include "Encoder.h"
#include "Text.h"

Encoder::Encoder(wchar_t *label, float default_value_, std::function<void(float)> callback_) :
Encoder(label, default_value_, callback_, 0) {}

Encoder::Encoder(wchar_t * label, float default_value_, std::function<void(float)> callback_,
                 unsigned int default_map_){
    callback = callback_;

    GAttachTexture("Textures/effect_canvas.bmp");
    GSaveRatio(true);

    wheel = new GCanvas();
    wheel->GAttachShaders("Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlain.fsh");
    wheel->GAttachTexture("Textures/encoder.bmp");
    wheel->GSaveRatio(true);
    wheel->place(0.15, 0.05, 0.6, 0.6);
    GAttach(wheel);
    keymap = default_map_;
    setvalue(default_value_);

    auto txt = new Text("Fonts/Roboto-Regular.ttf", label);
    txt->place(0.07, 0.77, 0.18, 0.9);
    GAttach(txt);

    info_overlay.GAttachShaders("Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainColor.fsh");
    info_overlay.GSetColor(0, 0, 1, 0.2);
    info_overlay.place(0, 0, 1, 1);
    info_overlay.GSaveRatio(true);
    GAttach(&info_overlay);
}

void Encoder::GSetVisible(bool visible_) {
    GObject::GSetVisible(visible_);
    info_overlay.GSetVisible(false);
}

void Encoder::GDragHandler(const ndk_helper::Vec2 &v) {
    wheel->angle = old_angle + (v.x_ - drag_from.x_)/100;
    setangle(wheel->angle);
}

void Encoder::GDragBegin(const ndk_helper::Vec2 &v) {
    drag_from = v;
    old_angle = wheel->angle;
}