//
// Created by Admin on 04.09.2019.
//

#include "Led.h"

namespace GUI {

    void Led::GDraw(NVGcontext *nvg) {
        nvgBeginPath(nvg);
        nvgRect(nvg,
                shape->global.c.x - shape->global.s.x / 2,
                shape->global.c.y - shape->global.s.y / 2,
                shape->global.s.x, shape->global.s.y);
        if (state) {
            nvgFillColor(nvg, YELLOW);
        } else {
            nvgFillColor(nvg, GREY);
        }
        nvgFill(nvg);
        nvgClosePath(nvg);
    }

}