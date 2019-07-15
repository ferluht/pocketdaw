//
// Created by ibelikov on 15.07.19.
//

#ifndef PD_CANVAS_H
#define PD_CANVAS_H


#include "GraphicObject.h"

class Canvas : public GraphicObject{
public:

    Canvas(float x, float y, float h, float w) : GraphicObject()
    {
        position.x = x;
        position.y = y;
        position.height = h;
        position.width = w;
    }

    void draw() override ;
    void grender(float dTime) override ;
};


#endif //PD_CANVAS_H
