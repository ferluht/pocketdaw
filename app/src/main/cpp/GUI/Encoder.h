//
// Created by Admin on 06.07.2019.
//

#ifndef PD_ENCODER_H
#define PD_ENCODER_H

#include "GraphicObject.h"

#define SEGMENTS 30

class Encoder : public GraphicObject{
public:
    float * parameter_;

    float angle = 0;
    float old_angle;

    Encoder(const char * texture, float x, float y, float * parameter);

    void SetPosition(float x, float y);
    void SetSize(float x, float y);
    void grender(float dTime) override ;
    void dragHandler(ndk_helper::Vec2 v) override ;
    void dragBegin(ndk_helper::Vec2 v, float xscale, float yscale) override ;
};


#endif //PD_ENCODER_H
