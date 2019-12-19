//
// Created by Admin on 13.08.2019.
//

#include "Knob.h"
#include "styles.h"

namespace GUI {

    void Knob::GDraw(NVGcontext *nvg) {

        if (mapping_mode) {
            nvgBeginPath(nvg);
            nvgRect(nvg, shape->global.c.x, shape->global.c.y, shape->global.s.x,
                    shape->global.s.y);
            auto color = BLUE;
            color.a = 0.2;
            nvgFillColor(nvg, color);
            nvgFill(nvg);
            nvgClosePath(nvg);
        }
    }

}