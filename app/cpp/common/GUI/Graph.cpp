//
// Created by ibelikov on 17.07.19.
//

#include "Graph.h"

namespace GUI {


    void TimeGraph::update(float sample) {
        if (sample > 1) sample = 1;
        if (sample < -1) sample = -1;

        points[current_position] = sample / 2 + 0.5f;
        current_position = (current_position + 1) % number_of_points;
    }

    void TimeGraph::GDraw(NVGcontext * nvg) {

        unsigned int r = current_position;

        nvgBeginPath(nvg);

        nvgMoveTo(nvg, shape->global.c.x, shape->global.c.y + (1 - points[r]) * shape->global.s.y);
        r = (r + 1) % number_of_points;

        float dx = shape->global.s.x / (float)number_of_points;

        for (int i = 1; i < number_of_points; i++) {
            nvgLineTo(nvg, shape->global.c.x + i * dx, shape->global.c.y + (1 - points[r]) * shape->global.s.y);
            r = (r + 1) % number_of_points;
        };

        BaseGraph::GDraw(nvg);

        nvgClosePath(nvg);
    }

    void XYGraph::GDraw(NVGcontext * nvg) {

        nvgBeginPath(nvg);

        nvgMoveTo(nvg, shape->global.c.x + points[0] * shape->global.s.x, shape->global.c.y + (1 - points[1]) * shape->global.s.y);

        for (int i = 1; i < number_of_points; i++) {
            nvgLineTo(nvg,
                    shape->global.c.x + points[2 * i] * shape->global.s.x,
                    shape->global.c.y + points[2 * i + 1] * shape->global.s.y);
        };

        BaseGraph::GDraw(nvg);

        nvgClosePath(nvg);
    }

    void XYGraph::update(float x, float y) {
        if (x > 1) x = 1;
        if (x < -1) x = -1;
        if (y > 1) y = 1;
        if (y < -1) y = -1;

        points[2 * r] = x;
        points[2 * r + 1] = y;
        r = (r + 1) % number_of_points;
    }

}