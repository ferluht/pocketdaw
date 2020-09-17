//
// Created by ibelikov on 15.07.19.
//

#include "Canvas.h"

namespace GUI {

    void AMGCanvas::GDraw(NVGcontext * nvg) {

        nvgBeginPath(nvg);
        nvgRect(nvg, global.c.x, global.c.y, global.s.x, global.s.y);
        nvgFillColor(nvg, color);
        nvgFill(nvg);
        nvgClosePath(nvg);

    }

}