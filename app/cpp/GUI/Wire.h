//
// Created by ibelikov on 29.12.19.
//

#ifndef PD_WIRE_H
#define PD_WIRE_H

#include <AMGEngine/G.h>
#include "styles.h"

namespace GUI {

    class Wire : public GObject {

        GObject * a;
        GObject * b;
        Vec2 c;

        NVGcolor color;

    public:

        Wire() : GObject(BOX) {
            unsigned char R = rand() % 255;
            unsigned char G = rand() % 255;
            unsigned char B = rand() % 255;
            color = nvgRGB(R, G, B);
            a = nullptr;
            b = nullptr;
            auto & eng = GEngine::getGEngine();
            eng.addOverlay(this);
        }

        ~Wire() {
            auto & eng = GEngine::getGEngine();
            eng.delOverlay(this);
        }

        inline void connect(GObject * a_, GObject * b_) {
            a = a_;
            b = b_;
            c = {0, 0};
            auto & eng = GEngine::getGEngine();
            eng.addOverlay(this);
        }

        inline void drawPreviewTo(GObject * a_, Vec2 v) {
            a = a_;
            c = v;
        }

        void GDraw(NVGcontext * nvg) override {

            float a_x = a->global.c.x + a->global.s.x / 2;
            float a_y = a->global.c.y + a->global.s.y / 2;

            float b_x = 0;
            float b_y = 0;

            if (b) {
                b_x = b->global.c.x + b->global.s.x / 2;
                b_y = b->global.c.y + b->global.s.y / 2;
            }

            // Shadow

            nvgBeginPath(nvg);
            nvgMoveTo(nvg, a_x, a_y + a->global.s.y * 0.5f);
            nvgStrokeColor(nvg, DARKER);
            nvgStrokeWidth(nvg, a->global.s.x * 0.2f);
            if (c.x != 0 && c.y != 0) nvgBezierTo(nvg, a_x, a_y + 120, c.x, c.y + 200, c.x, c.y + a->global.s.y * 0.5f);
            else nvgBezierTo(nvg, a_x, a_y + 120, b_x, b_y + 200, b_x, b_y + b->global.s.y * 0.5f);
            nvgStroke(nvg);
            nvgClosePath(nvg);

            // Wire

            nvgBeginPath(nvg);
            nvgMoveTo(nvg, a_x, a_y + a->global.s.y * 0.5f);
            nvgStrokeColor(nvg, color);
            if (c.x != 0 && c.y != 0) nvgBezierTo(nvg, a_x, a_y + 100, c.x, c.y + 100, c.x, c.y + a->global.s.y * 0.5f);
            else nvgBezierTo(nvg, a_x, a_y + 100, b_x, b_y + 100, b_x, b_y + b->global.s.y * 0.5f);
            nvgStroke(nvg);
            nvgClosePath(nvg);

            // A circle

            nvgBeginPath(nvg);
            nvgCircle(nvg, a_x, a_y, a->global.s.x * 0.25f);
            nvgStrokeColor(nvg, color);
            nvgStrokeWidth(nvg, a->global.s.x * 0.1f);
            nvgStroke(nvg);
            nvgClosePath(nvg);

            // B circle

            nvgBeginPath(nvg);
            if (c.x != 0 && c.y != 0) {
                nvgCircle(nvg, c.x, c.y, a->global.s.x * 0.25f);
                nvgStrokeWidth(nvg, a->global.s.x * 0.1f);
            } else {
                nvgCircle(nvg, b_x, b_y, b->global.s.x * 0.25f);
                nvgStrokeWidth(nvg, b->global.s.x * 0.1f);
            }
            nvgStrokeColor(nvg, color);
            nvgStroke(nvg);
            nvgClosePath(nvg);

            nvgStrokeWidth(nvg, 1);
        }

    };

}


#endif //PD_WIRE_H
