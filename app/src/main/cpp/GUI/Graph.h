//
// Created by ibelikov on 17.07.19.
//

#ifndef PD_GRAPH_H
#define PD_GRAPH_H


#include "Engine/Engine.h"

class BaseGraph : public GObject{

public:

    unsigned int r;
    unsigned int buffer_size;
    float * buffer;
    GLfloat * g_vertex_buffer_data;

    BaseGraph() {
        GAttachShaders("Shaders/BasicV.vsh", "Shaders/BasicF.fsh");
        GSetColor(0, 1, 0, 1);
    }

    ~BaseGraph() {
        delete buffer;
        delete g_vertex_buffer_data;
    }

    virtual void fillGLBuffer() {};
    void GDraw() override ;
    void GRender(float dTime) override ;
};


class TimeGraph : public BaseGraph{

public:

    TimeGraph(unsigned int points)
    {
        this->buffer = new float[points];
        this->g_vertex_buffer_data = new GLfloat[points*2];
        this->buffer_size = points;
        this->r = 0;
    }

    void update(float sample);
    void fillGLBuffer() override ;
};

class XYGraph : public BaseGraph{

public:

    XYGraph(unsigned int points)
    {
        this->buffer = new float[points*2];
        this->g_vertex_buffer_data = new GLfloat[points*2];
        this->buffer_size = points;
        this->r = 0;
        color[0] = 0, color[1] = 1;
    }

    void update(float x, float y);
    void fillGLBuffer() override ;
};


#endif //PD_GRAPH_H
