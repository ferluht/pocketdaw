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

        BaseGraph::GDraw(nvg);

        unsigned int r = current_position;

        nvgBeginPath(nvg);

        nvgMoveTo(nvg, shape->global.c.x, shape->global.c.y + (1 - points[r]) * shape->global.s.y);
        r = (r + 1) % number_of_points;

        float dx = shape->global.s.x / (float)number_of_points;

        for (int i = 1; i < number_of_points; i++) {
            nvgLineTo(nvg, shape->global.c.x + i * dx, shape->global.c.y + (1 - points[r]) * shape->global.s.y);
            r = (r + 1) % number_of_points;
        };

        nvgStrokeColor(nvg, GREEN);
        nvgStrokeWidth(nvg, 2);
        nvgStroke(nvg);

        nvgClosePath(nvg);
    }

//    void XYGraph::GDraw(NVGcontext * nvg) {
//        for (int i = 0; i < buffer_size; i++) {
//            g_vertex_buffer_data[i * 2] = globalPosition.x + buffer[2 * r] * globalPosition.width;
//            g_vertex_buffer_data[i * 2 + 1] =
//                    globalPosition.y + buffer[2 * r + 1] * globalPosition.height;
//            r++;
//            if (r >= buffer_size) r = 0;
//        };
//    }
//
//    void XYGraph::update(float x, float y) {
//        changed = true;
//
//        if (x > 1) x = 1;
//        if (x < -1) x = -1;
//        if (y > 1) y = 1;
//        if (y < -1) y = -1;
//
//        buffer[2 * r] = x;
//        buffer[2 * r + 1] = y;
//        r = (r + 1) % buffer_size;
//    }

}