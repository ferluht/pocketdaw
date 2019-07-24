//
// Created by ibelikov on 15.07.19.
//

#ifndef PD_CANVAS_H
#define PD_CANVAS_H


#include "GUI/GraphicObject.h"

class Canvas : public GraphicObject{
public:

    Canvas() : Canvas(0, 0, -1, -1) {}

    Canvas(float x, float y, float h, float w) : Canvas(x, y, h, w, nullptr) {}

    Canvas(float x, float y, float h, float w, const char * texture_)
    : Canvas(x, y, h, w, texture_, "Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainRect.fsh") {}

    Canvas(float x, float y, float h, float w, const char * texture_, const char * vsh, const char * fsh)
    : GraphicObject(texture_, vsh, fsh)
    {
        this->x = x;
        this->y = y;
        this->height = h;
        this->width = w;
        this->angle = 0;
    }

    void init() override ;
    void draw() override ;
    void grender(float dTime) override ;
};


#endif //PD_CANVAS_H
 