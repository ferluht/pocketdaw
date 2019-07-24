//
// Created by ibelikov on 17.07.19.
//

#ifndef PD_GRAPH_H
#define PD_GRAPH_H


#include "GraphicObject.h"

class SimpleGraph : public GraphicObject{

private:

    unsigned int r;
    unsigned int buffer_size;
    float * buffer;
    GLfloat * g_vertex_buffer_data;

public:

    SimpleGraph(unsigned int points, float x, float y, float h, float w)
        : GraphicObject("BasicV.vsh", "BasicF.fsh")
    {
        this->buffer = new float[points];
        this->g_vertex_buffer_data = new GLfloat[points];
        this->buffer_size = points;
        this->x = x;
        this->y = y;
        this->height = h;
        this->width = w;
        this->angle = 0;
        this->r = 0;
    }

    ~SimpleGraph() {
        delete buffer;
        delete g_vertex_buffer_data;
    }

    void draw() override ;
    void grender(float dTime) override ;
};


#endif //PD_GRAPH_H
