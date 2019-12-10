//
// Created by ibelikov on 15.07.19.
//

#include "Canvas.h"

namespace GUI {

    void GCanvas::GDraw(NVGcontext * nvg) {

        nvgBeginPath(nvg);
        nvgRect(nvg,
        shape->global.c.x,
        shape->global.c.y,
        shape->global.s.x, shape->global.s.y);
        nvgFillColor(nvg, color);
        nvgFill(nvg);
        nvgClosePath(nvg);

//        vd->drawBox(shape->global.c.x - shape->global.s.x/2,
//                    shape->global.c.y - shape->global.s.y/2,
//                    shape->global.s.x,
//                    shape->global.s.y);
    }

    void GCanvas::GRender(NVGcontext * nvg, float dTime) {

    }

}