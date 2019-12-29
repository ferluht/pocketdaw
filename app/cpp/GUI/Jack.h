//
// Created by ibelikov on 29.12.19.
//

#ifndef PD_JACK_H
#define PD_JACK_H

#include <AMGEngine/AMGEngine.h>
#include "styles.h"
#include "Wire.h"

namespace GUI {

    class Jack;

    static auto isJack = GUI::IsType<Jack>;

    class Jack : public AMGObject {

        std::list<Wire *> wires;

    public:

        Jack() : AMGObject(CIRCLE) {
            GSetRatio(1);
        }

        GObject *GDragBegin(const Vec2 &v) override {
            wires.push_back(new Wire());
            wires.back()->drawPreviewTo(this, v);
            return this;
        }

        GObject *GDragHandler(const Vec2 &v) override {
            wires.back()->drawPreviewTo(this, v);
            return this;
        }

        GObject *GDragEnd(const Vec2 &v) override {

            auto & eng = GEngine::getGEngine();

            std::list<GObject *> trace;
            auto obj = eng.focusStack.front()->GFindFocusObject(v, &trace);

            Wire * w = wires.back();

            if (isJack(obj) && obj != this) {
                w->connect(this, obj);
            } else {
                wires.pop_back();
                delete w;
            }
            return this;
        }

        void GDraw(NVGcontext * nvg) override {

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

            nvgStrokeWidth(nvg, 1);
        }

    };

}

#endif //PD_JACK_H
