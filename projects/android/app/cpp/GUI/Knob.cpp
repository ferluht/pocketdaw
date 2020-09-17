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
                    nvgRect(nvg, global.c.x, global.c.y, global.s.x,
                            global.s.y);
                    break;
                case CIRCLE:
                    nvgCircle(nvg, global.c.x + global.s.x / 2, global.c.y + global.s.y / 2, global.s.x / 2);
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