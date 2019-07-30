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

    SimpleGraph(unsigned int points, float x_, float y_, float z_, float h_, float w_)
        : GraphicObject(x_, y_, z_, h_, w_, 0, nullptr, "Shaders/BasicV.vsh", "Shaders/BasicF.fsh", false)
    {
        this->buffer = new float[points];
        this->g_vertex_buffer_data = new GLfloat[points*3];
        this->buffer_size = points;
        this->r = 0;
    }

    ~SimpleGraph() {
        delete buffer;
        delete g_vertex_buffer_data;
    }

    void update(float sample);
    void draw() override ;
    void grender(float dTime) override ;
};


class XYGraph : public GraphicObject{

private:

    unsigned int r;
    unsigned int buffer_size;
    float * buffer;
    GLfloat * g_vertex_buffer_data;

public:

    XYGraph(unsigned int points, float x_, float y_, float z_, float h_, float w_)
            : GraphicObject(x_, y_, z_, h_, w_, 0, nullptr, "Shaders/TrailV.vsh", "Shaders/TrailF.fsh", false)
    {
        this->buffer = new float[points*2];
        this->g_vertex_buffer_data = new GLfloat[points*4];
        this->buffer_size = points;
        this->r = 0;
    }

    ~XYGraph() {
        delete buffer;
        delete g_vertex_buffer_data;
    }

    void update(float x, float y);
    void draw() override ;
    void grender(float dTime) override ;
};


#endif //PD_GRAPH_H
