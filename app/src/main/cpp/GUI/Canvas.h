//
// Created by ibelikov on 15.07.19.
//

#ifndef PD_CANVAS_H
#define PD_CANVAS_H


#include "GraphicObject.h"

class Canvas : public GraphicObject{
public:

    Canvas(float x, float y, float h, float w, const char * texture_)
    : GraphicObject(texture_, "Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainRect.fsh")
    {
        this->x = x;
        this->y = y;
        this->height = h;
        this->width = w;
        this->angle = 0;
    }

    void draw() override ;
    void grender(float dTime) override ;
};


#endif //PD_CANVAS_H
