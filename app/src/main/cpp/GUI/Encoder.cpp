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
    setRatio(0.8);

    wheel = new GCanvas();
    wheel->GAttachShaders("Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlain.fsh");
    wheel->GAttachTexture("Textures/encoder.bmp");
    wheel->GSaveRatio(true);
    wheel->place(0.05, 0.05);
    wheel->setWidth(0.9);
    wheel->setRatio(1);
    GAttach(wheel);
    keymap = default_map_;
    setvalue(default_value_);

    auto txt = new Text("Fonts/Roboto-Regular.ttf", label);
    txt->place(0.07, 0.77);
    txt->setHeight(0.15);
    GAttach(txt);
}

void Encoder::GSetVisible(bool visible_) {
    GObject::GSetVisible(visible_);
    info_overlay.GSetVisible(false);
}

GObject * Encoder::GDragHandler(const ndk_helper::Vec2 &v) {
    wheel->angle = old_angle + (v.x_ - drag_from.x_)/100;
    setangle(wheel->angle);
    return this;
}

GObject * Encoder::GDragBegin(const ndk_helper::Vec2 &v) {
    drag_from = v;
    old_angle = wheel->angle;
    return this;
}