//
// Created by ibelikov on 17.07.19.
//

#ifndef PD_GRAPH_H
#define PD_GRAPH_H


#include "Engine/Engine.h"
#include "Canvas.h"
#include "styles.h"

namespace GUI {

    class BaseGraph : public GCanvas {

    public:

        unsigned int current_position;
        unsigned int number_of_points;
        float * points;

        BaseGraph() {
            setColor(BLACK);
        }

        ~BaseGraph() {
            delete points;
        }
    };


    class TimeGraph : public BaseGraph {

    public:

        TimeGraph(unsigned int num_points_) {
            number_of_points = num_points_;
            points = new float[number_of_points * 2];
            current_position = 0;
        }

        void update(float sample);

        void GDraw(NVGcontext * nvg) override ;
    };

//    class XYGraph : public BaseGraph {
//
//    public:
//
//        XYGraph(unsigned int points) {
//            this->buffer = new float[points * 2];
//            this->g_vertex_buffer_data = new GLfloat[points * 2];
//            this->buffer_size = points;
//            this->r = 0;
//            color[0] = 0, color[1] = 1;
//        }
//
//        void update(float x, float y);
//
//        void fillGLBuffer() override;
//    };

}

#endif //PD_GRAPH_H
