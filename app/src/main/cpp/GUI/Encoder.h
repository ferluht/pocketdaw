//
// Created by Admin on 06.07.2019.
//

#ifndef PD_ENCODER_H
#define PD_ENCODER_H

#include "Canvas.h"

class Encoder : public AMGCanvas{
public:

    std::function<void(float)> callback;

    float old_angle;

    Encoder(float default_value_, std::function<void(float)> callback_);

    void GDragHandler(const ndk_helper::Vec2& v) override ;
    void GDragBegin(const ndk_helper::Vec2& v) override ;
};


#endif //PD_ENCODER_H
