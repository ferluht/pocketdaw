//
// Created by ibelikov on 16.12.19.
//

#include "Slider.h"


namespace GUI {

    Slider::Slider(const char *label_, float val_, float min_, float max_) : Knob(BOX) {

        size_t len = strlen(label_);
        label = new char[len + 1];
        strncpy(label, label_, len);
        label[len] = 0;

        value = val_;

        min = min_;
        max = max_;
    }

    void Slider::GDraw(NVGcontext *nvg) {
        nvgBeginPath(nvg);
        nvgRect(nvg,
                global.c.x,
                global.c.y,
                global.s.x, global.s.y);
        nvgStrokeColor(nvg, BLACK);
        nvgStroke(nvg);
        nvgClosePath(nvg);

        nvgBeginPath(nvg);
        nvgMoveTo(nvg, global.c.x + global.s.x/2, global.c.y + global.s.y * top_text_height * 2);
        nvgLineTo(nvg, global.c.x + global.s.x/2, global.c.y + global.s.y * (1 - bottom_text_height * 2));
        nvgStrokeColor(nvg, BLACK);
        nvgStrokeWidth(nvg, 2);
        nvgStroke(nvg);
        nvgClosePath(nvg);

        nvgBeginPath(nvg);
        nvgFontSize(nvg, global.s.y * top_text_height);
        nvgFontFace(nvg, "sans");
        nvgTextAlign(nvg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

        nvgFillColor(nvg, BLACK);
        nvgText(nvg, global.c.x + global.s.x/2, global.c.y + top_text_height * global.s.y, label, NULL);
        nvgClosePath(nvg);

        nvgBeginPath(nvg);
        nvgFontSize(nvg, global.s.y * bottom_text_height);
        nvgFontFace(nvg, "sans");
        nvgTextAlign(nvg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

        char buffer[64];
        snprintf(buffer, sizeof buffer, "%.2f", value);

        nvgFillColor(nvg, RED);
        nvgText(nvg, global.c.x + global.s.x/2, global.c.y + global.s.y * (1 - bottom_text_height), buffer, NULL);
        nvgBeginPath(nvg);

        nvgBeginPath(nvg);
        nvgRect(nvg,
                global.c.x + (1 - slider_width) / 2 * global.s.x,
                global.c.y + global.s.y * (top_text_height * 2 - slider_height / 2 + (1 - value) / 2 * (1 - bottom_text_height * 2 - top_text_height * 2)),
                global.s.x * slider_width, global.s.y * slider_height);
        nvgFillColor(nvg, GREY);
        nvgFill(nvg);
        nvgClosePath(nvg);
    }

    GObject *Slider::GDragHandler(const Vec2 &v) {
        value = old_value - (v.y - drag_from.y) / 100;

        if (value < min) value = min;
        if (value > max) value = max;

        return this;
    }

    GObject *Slider::GDragBegin(const Vec2 &v) {
        drag_from = v;
        old_value = value;
        return this;
    }

}