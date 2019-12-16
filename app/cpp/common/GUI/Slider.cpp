//
// Created by ibelikov on 16.12.19.
//

#include "Slider.h"


namespace GUI {

    Slider::Slider(const char *label_, float val_, float min_, float max_) {
        setShapeType(BOX);

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
                shape->global.c.x,
                shape->global.c.y,
                shape->global.s.x, shape->global.s.y);
        nvgStrokeColor(nvg, BLACK);
        nvgStroke(nvg);
        nvgClosePath(nvg);

        nvgBeginPath(nvg);
        nvgMoveTo(nvg, shape->global.c.x + shape->global.s.x/2, shape->global.c.y + shape->global.s.y * top_text_height * 2);
        nvgLineTo(nvg, shape->global.c.x + shape->global.s.x/2, shape->global.c.y + shape->global.s.y * (1 - bottom_text_height * 2));
        nvgStrokeColor(nvg, BLACK);
        nvgStrokeWidth(nvg, 2);
        nvgStroke(nvg);
        nvgClosePath(nvg);

        nvgBeginPath(nvg);
        nvgFontSize(nvg, shape->global.s.y * top_text_height);
        nvgFontFace(nvg, "sans");
        nvgTextAlign(nvg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

        nvgFillColor(nvg, BLACK);
        nvgText(nvg, shape->global.c.x + shape->global.s.x/2, shape->global.c.y + top_text_height * shape->global.s.y, label, NULL);
        nvgClosePath(nvg);

        nvgBeginPath(nvg);
        nvgFontSize(nvg, shape->global.s.y * bottom_text_height);
        nvgFontFace(nvg, "sans");
        nvgTextAlign(nvg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

        char buffer[64];
        snprintf(buffer, sizeof buffer, "%.2f", value);

        nvgFillColor(nvg, RED);
        nvgText(nvg, shape->global.c.x + shape->global.s.x/2, shape->global.c.y + shape->global.s.y * (1 - bottom_text_height), buffer, NULL);
        nvgBeginPath(nvg);

        nvgBeginPath(nvg);
        nvgRect(nvg,
                shape->global.c.x + (1 - slider_width) / 2 * shape->global.s.x,
                shape->global.c.y + shape->global.s.y * (top_text_height * 2 - slider_height / 2 + (1 - value) / 2 * (1 - bottom_text_height * 2 - top_text_height * 2)),
                shape->global.s.x * slider_width, shape->global.s.y * slider_height);
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