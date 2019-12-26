//
// Created by ibelikov on 24.12.19.
//

#include "EncoderButton.h"

namespace GUI {

    void EncoderButton::GDraw(NVGcontext *nvg) {

        drawArc(nvg);
        drawButton(nvg);
        drawLed(nvg);
        Knob::GDraw(nvg);
    }

    void EncoderButton::drawLed(NVGcontext *nvg) {
        Vec2 led_center(global.c.x + global.s.x/2, global.c.y + global.s.y/2);
        float led_radius = global.s.x * 0.2f;

        nvgBeginPath(nvg);
        nvgCircle(nvg, led_center.x, led_center.y, led_radius);
        if (state) {
            nvgFillColor(nvg, YELLOW);
        } else {
            nvgFillColor(nvg, GREY);
        }
        nvgFill(nvg);
        if (lighted) {
            nvgFillColor(nvg, lightColor);
            nvgFill(nvg);
        }
        nvgFill(nvg);
        nvgClosePath(nvg);
    }

    void EncoderButton::drawButton(NVGcontext *nvg) {
        Vec2 button_center(global.c.x + global.s.x/2, global.c.y + global.s.y/2);
        float button_radius = global.s.x * 0.4f;

        nvgBeginPath(nvg);
        nvgCircle(nvg, button_center.x, button_center.y, button_radius);
        nvgFillColor(nvg, DARKER);
        nvgFill(nvg);
        nvgClosePath(nvg);
    }

    void EncoderButton::drawArc(NVGcontext *nvg) {
        Vec2 button_center(global.c.x + global.s.x/2, global.c.y + global.s.y/2);
        float button_radius = global.s.x * 0.4f;

        nvgBeginPath(nvg);
        nvgArc(nvg, button_center.x, button_center.y, button_radius, M_PI * (0.5f + 2 * (value - lower_bound)/(upper_bound - lower_bound)), M_PI * 0.5f, NVG_CCW);
        nvgStrokeWidth(nvg, global.s.x * 0.1f);
        nvgStrokeColor(nvg, VIOLET);
        nvgStroke(nvg);
        nvgClosePath(nvg);
    }

    GObject * EncoderButton::GTapEnd(const ndk_helper::Vec2 &v) {
        state = !state;
        return nullptr;
    }

}