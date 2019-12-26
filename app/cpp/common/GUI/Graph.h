//
// Created by ibelikov on 17.07.19.
//

#ifndef PD_GRAPH_H
#define PD_GRAPH_H


#include "Engine/Engine.h"
#include "Canvas.h"
#include "styles.h"

namespace GUI {

    class BaseGraph : public AMGObject {

        NVGcolor line_color;
        int width;

    public:

        unsigned int current_position;
        unsigned int number_of_points;
        float * points;

        BaseGraph() : AMGObject(BOX) {
            line_color = GREEN;
            width = 2;
        }

        ~BaseGraph() {
            delete points;
        }

        inline void setLineColor(NVGcolor line_color_) {
            line_color = line_color_;
        }

        inline void setLineWidth(int width_) {
            width = width_;
        }

        virtual inline void GDraw(NVGcontext * nvg) override {
            nvgStrokeColor(nvg, line_color);
            nvgStrokeWidth(nvg, width);
            nvgStroke(nvg);
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

    class XYGraph : public BaseGraph {

        int r;

    public:

        XYGraph(unsigned int num_points_) {
            number_of_points = num_points_;
            points = new float[num_points_ * 2];

            r = 0;
        }

        void update(float x, float y);

        void GDraw(NVGcontext * nvg) override ;
    };

}

#endif //PD_GRAPH_H
