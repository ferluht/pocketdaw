//
// Created by ibelikov on 29.12.19.
//

#ifndef PD_JACK_H
#define PD_JACK_H

#include <AMGEngine.h>
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

        float value;

    public:

        static const float RATIO;

        static enum {
            INPUT,
            OUTPUT
        };

        Jack(int type_) : AMGObject(CIRCLE) {
            GSetRatio(RATIO);
            type = type_;
            value = 0;

            GSetDragBeginCallback([this](const Vec2& v) -> GUI::GObject * {
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
            });

            GSetDragHandlerCallback([this](const Vec2& v) -> GUI::GObject * {
                focusWire->from(this);
                focusWire->to(v);
                return this;
            });

            GSetDragEndCallback([this](const Vec2& v) -> GUI::GObject * {
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
            });
        }

        bool isConnected() {
            if (type == INPUT) return input.first != nullptr;
            return !outputs.empty();
        }

        Jack &operator=(const float &value_) {
            value = value_;
            return *this;
        }

        Jack &operator=(const double &value_) {
            value = (float)value_;
            return *this;
        }

        Jack &operator=(const int &value_) {
            value = value_;
            return *this;
        }

        void GSetVisible(bool visible_) override {
            for (const auto & out : outputs) out.first->GSetVisible(visible_);
            if (input.first != nullptr) input.first->GSetVisible(visible_);
        }

        operator float() const { return value; }

        inline void MRender(double beat) override { for (const auto & out : outputs) *out.second = value; }

        void GDraw(NVGcontext * nvg) override ;

    };

}

#endif //PD_JACK_H
