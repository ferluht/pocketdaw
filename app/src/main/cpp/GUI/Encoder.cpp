//
// Created by Admin on 06.07.2019.
//

#include "Encoder.h"

Encoder::Encoder(float default_value_, float x_, float y_, float z_, float h_, const char * texture_, std::function<void(float)> callback_)
: Canvas(x_, y_, z_, h_, h_, -1.25f*(float)M_PI + default_value_*1.5*(float)M_PI, texture_, "Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlain.fsh", true){
    callback = callback_;
}

void Encoder::dragHandler(const ndk_helper::Vec2 &v) {
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

void Encoder::dragBegin(const ndk_helper::Vec2 &v) {
    drag_from = v;
    old_angle = angle;
}