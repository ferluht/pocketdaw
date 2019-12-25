//
// Created by Admin on 13.08.2019.
//

#include "Knob.h"
#include "styles.h"

namespace GUI {

    void Knob::GDraw(NVGcontext *nvg) {

        if (mapping_mode) {
            nvgBeginPath(nvg);

            switch (overlay_type) {
                case BOX:
                    nvgRect(nvg, shape->global.c.x, shape->global.c.y, shape->global.s.x,
                            shape->global.s.y);
                    break;
                case CIRCLE:
                    nvgCircle(nvg, shape->global.c.x + shape->global.s.x / 2, shape->global.c.y + shape->global.s.y / 2, shape->global.s.x / 2);
                    break;
                default:
                    return;
            }

            auto color = BLUE;
            color.a = 0.2;
            nvgFillColor(nvg, color);
            nvgFill(nvg);
            nvgClosePath(nvg);
        }
    }



}