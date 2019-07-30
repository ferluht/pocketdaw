//
// Created by Admin on 06.07.2019.
//

#ifndef PD_ENCODER_H
#define PD_ENCODER_H

#include "Canvas.h"

class Encoder : public Canvas{
public:

    std::function<void(float)> callback;

    float old_angle;

    Encoder(float default_value_, float x_, float y_, float z_, float h_, const char * texture_, std::function<void(float)> callback_);

    void dragHandler(const ndk_helper::Vec2& v) override ;
    void dragBegin(const ndk_helper::Vec2& v) override ;
};


#endif //PD_ENCODER_H
