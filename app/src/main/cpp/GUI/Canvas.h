//
// Created by ibelikov on 15.07.19.
//

#ifndef PD_CANVAS_H
#define PD_CANVAS_H


#include "GUI/GraphicObject.h"

class Canvas : public GraphicObject{
public:

    Canvas() : Canvas(0, 0, 1, 1) {}

    Canvas(float x, float y, float h, float w) : Canvas(x, y, h, w, nullptr, false) {}

    Canvas(float x, float y, float h, float w, const char * texture_, bool saveRatio_)
    : Canvas(x, y, 0, h, w, 0, texture_, "Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainRect.fsh", saveRatio_) {}

    Canvas(float x_, float y_, float z_, float h_, float w_, float angle_, const char * texture_,
            const char * vsh_, const char * fsh_, bool saveRatio_)
            : GraphicObject(x_, y_, z_, h_, w_, angle_, texture_, vsh_, fsh_, saveRatio_) {}

    void draw() override ;
    void grender(float dTime) override ;
};


#endif //PD_CANVAS_H
 