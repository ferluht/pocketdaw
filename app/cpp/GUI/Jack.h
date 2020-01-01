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

        operator float() const { return value; }

        inline void MRender(double beat) override { for (const auto & out : outputs) *out.second = value; }

        GObject *GDragBegin(const Vec2 &v) override ;

        GObject *GDragHandler(const Vec2 &v) override ;

        GObject *GDragEnd(const Vec2 &v) override ;

        void GDraw(NVGcontext * nvg) override ;

    };

}

#endif //PD_JACK_H
