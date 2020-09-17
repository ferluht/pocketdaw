//
// Created by ibelikov on 29.12.19.
//

#include "Jack.h"

namespace GUI {

    const float Jack::RATIO = 1;

    void Jack::GDraw(NVGcontext *nvg) {
        Vec2 wheel_center(global.c.x + global.s.x/2, global.c.y + global.s.y / 2);
        float wheel_radius = global.s.x * 0.5f;

        nvgBeginPath(nvg);
        nvgCircle(nvg, wheel_center.x, wheel_center.y, wheel_radius * 0.9);
        nvgStrokeColor(nvg, DARKER);
        nvgStrokeWidth(nvg, wheel_radius * 0.15);
        nvgStroke(nvg);
        nvgClosePath(nvg);

        nvgBeginPath(nvg);
        nvgCircle(nvg, wheel_center.x, wheel_center.y, wheel_radius * 0.8);
        nvgStrokeColor(nvg, GREY);
        nvgStrokeWidth(nvg, wheel_radius * 0.15);
        nvgStroke(nvg);
        nvgClosePath(nvg);

        nvgBeginPath(nvg);
        nvgCircle(nvg, wheel_center.x, wheel_center.y, wheel_radius * 0.7);
        nvgFillColor(nvg, BLACK);
        nvgFill(nvg);
        nvgClosePath(nvg);

        if (isConnected()) {
            NVGcolor led = GREEN;
            led.a = (value + 1) / 2;
            nvgBeginPath(nvg);
            nvgCircle(nvg, wheel_center.x, wheel_center.y, wheel_radius * 0.4);
            nvgFillColor(nvg, led);
            nvgFill(nvg);
            nvgClosePath(nvg);
        }

        nvgStrokeWidth(nvg, 1);
    }

}