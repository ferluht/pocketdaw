//
// Created by Admin on 04.09.2019.
//

#include "Led.h"

namespace GUI {

    void Led::GDraw(NVGcontext *nvg) {
        nvgBeginPath(nvg);
        nvgRect(nvg,
                global.c.x - global.s.x / 2,
                global.c.y - global.s.y / 2,
                global.s.x, global.s.y);
        if (state) {
            nvgFillColor(nvg, YELLOW);
        } else {
            nvgFillColor(nvg, GREY);
        }
        nvgFill(nvg);
        nvgClosePath(nvg);
    }

}