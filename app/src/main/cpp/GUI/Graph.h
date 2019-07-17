//
// Created by ibelikov on 17.07.19.
//

#ifndef PD_GRAPH_H
#define PD_GRAPH_H


#include "GraphicObject.h"

class Graph : public GraphicObject{
public:

    Graph(float x, float y, float h, float w, const char * texture_)
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


#endif //PD_GRAPH_H
