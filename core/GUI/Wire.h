//
// Created by ibelikov on 29.12.19.
//

#ifndef PD_WIRE_H
#define PD_WIRE_H

#include <G.h>
#include "styles.h"

namespace GUI {

    class Wire : public GObject {

        GObject * a_obj;
        GObject * b_obj;
        Vec2 a_vec;
        Vec2 b_vec;

        NVGcolor color;

        void drawShadow(NVGcontext * nvg, Vec2 a, Vec2 b, float rad_a, float rad_b) {
            nvgBeginPath(nvg);
            nvgMoveTo(nvg, a.x, a.y + rad_a);
            nvgStrokeColor(nvg, DARKER);
            nvgStrokeWidth(nvg, rad_a * 0.5f);
            nvgBezierTo(nvg, a.x - 0.2f * (a.x - b.x), a.y + 110, b.x + 0.2f * (a.x - b.x), b.y + 120, b.x, b.y + rad_b);
            nvgStroke(nvg);
            nvgClosePath(nvg);
        }

        void drawWire(NVGcontext * nvg, Vec2 a, Vec2 b, float rad_a, float rad_b) {
            nvgBeginPath(nvg);
            nvgMoveTo(nvg, a.x, a.y + rad_a);
            nvgStrokeColor(nvg, color);
            nvgStrokeWidth(nvg, rad_a * 0.5f);
            nvgBezierTo(nvg, a.x - 0.2f * (a.x - b.x), a.y + 100, b.x + 0.2f * (a.x - b.x), b.y + 100, b.x, b.y + rad_b);
            nvgStroke(nvg);
            nvgClosePath(nvg);
        }

        void drawCircle(NVGcontext * nvg, Vec2 a, float rad_a) {
            nvgBeginPath(nvg);
            nvgCircle(nvg, a.x, a.y, rad_a);
            nvgStrokeWidth(nvg, rad_a / 2);
            nvgStrokeColor(nvg, color);
            nvgStroke(nvg);
            nvgClosePath(nvg);
        }

    public:

        Wire() : GObject(BOX) {
            unsigned char R = rand() % 255;
            unsigned char G = rand() % 255;
            unsigned char B = rand() % 255;
            color = nvgRGB(R, G, B);
//            color = nvgRGB(0, 156, 245);
            a_obj = nullptr;
            b_obj = nullptr;
            a_vec = {0, 0};
            b_vec = {0, 0};
            auto & eng = GEngine::getGEngine();
            eng.addOverlay(this);
        }

        ~Wire() {
            auto & eng = GEngine::getGEngine();
            eng.delOverlay(this);
        }

        inline void from(GObject * f) { a_obj = f; }
        inline void from(Vec2 f) { a_obj = nullptr, a_vec = f; }

        inline void to(GObject * t) { b_obj = t; }
        inline void to(Vec2 t) { b_obj = nullptr, b_vec = t; }

        inline bool isVisible() { return a_obj && b_obj && (a_obj->visible && b_obj->visible); }

        void GDraw(NVGcontext * nvg) override {

//            if (!isVisible())
//                return;

            Vec2 a = a_vec;
            Vec2 b = b_vec;

            float a_rad = 0;
            float b_rad = 0;

            if (a_obj) {
                a.x = a_obj->global.c.x + a_obj->global.s.x / 2;
                a.y = a_obj->global.c.y + a_obj->global.s.y / 2;
                a_rad = a_obj->global.s.x / 4;
            }

            if (b_obj) {
                b.x = b_obj->global.c.x + b_obj->global.s.x / 2;
                b.y = b_obj->global.c.y + b_obj->global.s.y / 2;
                b_rad = b_obj->global.s.x / 4;
            }

            if (a_rad == 0 && b_rad != 0) {
                a_rad = b_rad;
            } else if (b_rad == 0 && a_rad != 0) {
                b_rad = a_rad;
            } else if (a_rad == 0 && b_rad == 0) {
                a_rad = 10;
                b_rad = 10;
            }

//            drawShadow(nvg, a, b, a_rad, b_rad);
            drawWire(nvg, a, b, a_rad, b_rad);
            drawCircle(nvg, a, a_rad);
            drawCircle(nvg, b, b_rad);
            nvgStrokeWidth(nvg, 1);
        }

    };

}


#endif //PD_WIRE_H
