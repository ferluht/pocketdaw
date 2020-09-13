//
// Created by ibelikov on 28.12.19.
//

#ifndef PD_TEXT_H
#define PD_TEXT_H

#include <AMGEngine/G.h>
#include <AMGEngine/AMGEngine.h>
#include "styles.h"

namespace GUI {

    class Text : public AMGObject {

        char text[100];
        NVGcolor color;
        int align;

    public:

        Text(const char * text_, NVGcolor color_ = RED, int align_ = NVG_ALIGN_CENTER | NVG_ALIGN_LEFT) : AMGObject(BOX){
            size_t len = strlen(text_);
            strncpy(text, text_, len);
            text[len] = 0;
            color = color_;
            align = align_;
        }

        void TSetText(const char * text_) {
            size_t len = strlen(text_);
            strncpy(text, text_, len);
            text[len] = 0;
        }

        void GDraw(NVGcontext * nvg) override {
            nvgBeginPath(nvg);
            nvgFontSize(nvg, global.s.y/2);
            nvgFontFace(nvg, "sans");
            nvgTextAlign(nvg, align);
            nvgFillColor(nvg, color);
            if (align & NVG_ALIGN_LEFT)
                nvgText(nvg, global.c.x, global.c.y + global.s.y / 2, text, NULL);
            if (align & NVG_ALIGN_MIDDLE)
                nvgText(nvg, global.c.x + global.s.x /2, global.c.y + global.s.y / 2, text, NULL);
            nvgFill(nvg);
            nvgClosePath(nvg);
        }

    };

}


#endif //PD_TEXT_H
