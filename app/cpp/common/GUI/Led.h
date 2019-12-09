//
// Created by Admin on 04.09.2019.
//

#ifndef PD_LED_H
#define PD_LED_H

#include "Canvas.h"
#include "styles.h"

namespace GUI {

    class Led : public GCanvas {
    public:

        bool state;
        unsigned char counter;

        Led(bool state_) {
            setShapeType(BOX);
            shape->setRatio(1);
            counter = 0;
            state = state_;
        }

        inline void on() {
            state = true;
        }

        inline void off() {
            state = false;
        }

        inline void toggle() {
            if (state) off();
            else on();
        }

        inline void toggle(int divider) {
            counter++;
            if (counter >= divider) counter = 0;
            if (counter) return;
            if (state) off();
            else on();
        }

        operator bool() const { return state; }

        void GDraw(NVGcontext * nvg) override;
    };
}

#endif //PD_LED_H
