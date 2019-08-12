//
// Created by Admin on 06.07.2019.
//

#include "Encoder.h"

Encoder::Encoder(float default_value_, std::function<void(float)> callback_){
    callback = callback_;
    GAttachShaders("Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlain.fsh");
    GAttachTexture("Textures/encoder.bmp");
    GSaveRatio(true);
}

void Encoder::GDragHandler(const ndk_helper::Vec2 &v) {
    angle = old_angle + (v.x_ - drag_from.x_)/100;

    if (angle < -1.25f*(float)M_PI) {
        angle = -1.25f*(float)M_PI;
    }

    if (angle > 0.25f*(float)M_PI) {
        angle = 0.25f*(float)M_PI;
    }

    float param = (angle/(float)M_PI+0.5f)/0.75f;

    callback(param);
}

void Encoder::GDragBegin(const ndk_helper::Vec2 &v) {
    drag_from = v;
    old_angle = angle;
}