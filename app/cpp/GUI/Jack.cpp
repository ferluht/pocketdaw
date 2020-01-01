//
// Created by ibelikov on 29.12.19.
//

#include "Jack.h"

namespace GUI {

    const float Jack::RATIO = 1;

    GObject* Jack::GDragBegin(const Vec2 &v) {
        if (type == OUTPUT && !outputs.empty()) {
            auto wire = outputs.begin()->first;
            auto jack = outputs.begin()->second;
            jack->focusWire = wire;
            outputs.erase(wire);
            return jack;
        } else if (type == INPUT && input.first != nullptr) {
            auto jack = input.second;
            jack->focusWire = input.first;
            input = {nullptr, nullptr};
            return jack;
        }
        focusWire = new Wire();
        focusWire->from(this);
        focusWire->to(v);
        return this;
    }

    GObject* Jack::GDragHandler(const Vec2 &v) {
        focusWire->from(this);
        focusWire->to(v);
        return this;
    }

    GObject* Jack::GDragEnd(const Vec2 &v) {
        auto & eng = GEngine::getGEngine();
        std::list<GObject *> trace;
        Jack * jack = dynamic_cast<Jack *>(eng.focusStack.front()->GFindFocusObject(v, &trace));

        if (this->canConnect(jack)) connect(focusWire, jack);
        else {
            if (type == OUTPUT) outputs.erase(focusWire);
            else input = {nullptr, nullptr};
            delete focusWire;
        }
        return this;
    }

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