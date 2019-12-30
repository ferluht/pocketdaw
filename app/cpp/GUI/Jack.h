//
// Created by ibelikov on 29.12.19.
//

#ifndef PD_JACK_H
#define PD_JACK_H

#include <AMGEngine/AMGEngine.h>
#include "styles.h"
#include "Wire.h"

namespace GUI {

    class Jack : public AMGObject {

        std::map<Wire *, Jack *> outputs;
        std::pair<Wire *, Jack *> input;

        Wire * focusWire;

        int type;

        inline bool canConnect(Jack * jack) {
            return jack && (jack != this) && (jack->type != type)
                   && ((type == INPUT && input.first == nullptr) || (type == OUTPUT));
        }

        void connect(Wire * wire, Jack * jack) {
            wire->from(this);
            wire->to(jack);
            if (this->type == OUTPUT && jack->type == INPUT) {
                outputs.insert({wire, jack});
                jack->input = {wire, this};
            } else {
                input = {wire, jack};
                jack->outputs.insert({wire, this});
            }
        }

    public:

        static enum {
            INPUT,
            OUTPUT
        };

        float value;

        Jack(int type_) : AMGObject(CIRCLE) {
            GSetRatio(1);
            type = type_;
            value = 0;
        }

        inline void propagate(){
            for (const auto & out : outputs) {
                out.second->value = value;
            }
        }

        GObject *GDragBegin(const Vec2 &v) override {
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

        GObject *GDragHandler(const Vec2 &v) override {
            focusWire->from(this);
            focusWire->to(v);
            return this;
        }

        GObject *GDragEnd(const Vec2 &v) override {
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
