//
// Created by Admin on 06.07.2019.
//

#include "Encoder.h"

namespace GUI {

    Encoder::Encoder(char *label_, float default_value_) :
            Encoder(label_, default_value_, [](float value) {}, 0) {}

    Encoder::Encoder(char *label_, float default_value_, unsigned int default_map_) :
            Encoder(label_, default_value_, [](float value) {}, default_map_) {}

    Encoder::Encoder(char *label_, float default_value_, std::function<void(float)> callback_) :
            Encoder(label_, default_value_, callback_, 0) {}

    Encoder::Encoder(char *label_, float default_value_, float lower_bound_, float upper_bound_) :
            Encoder(label_, default_value_, [](float value) {}, 0, lower_bound_, upper_bound_) {}

    Encoder::Encoder(char *label_, float default_value_, std::function<void(float)> callback_,
                     unsigned int default_map_) :
            Encoder(label_, default_value_, callback_, default_map_, -1, 1) {}

    Encoder::Encoder(char *label_, float default_value_, unsigned int default_map_,
                     float lower_bound_, float upper_bound_) :
            Encoder(label_, default_value_, [](float value) {}, default_map_, lower_bound_,
                    upper_bound_) {}

    Encoder::Encoder(char *label_, float default_value_, std::function<void(float)> callback_,
                     float lower_bound_, float upper_bound_) :
            Encoder(label_, default_value_, callback_, 0, lower_bound_, upper_bound_) {}

    Encoder::Encoder(char *label_, float default_value_, std::function<void(float)> callback_,
                     unsigned int default_map_, float lower_bound_, float upper_bound_) {
        setShapeType(BOX);
        shape->setRatio(0.8);
        callback = std::move(callback_);
        lower_bound = lower_bound_;
        upper_bound = upper_bound_;

        size_t len = strlen(label_);
        label = new char[len + 1];
        strncpy(label, label_, len);

        keymap = default_map_;
        setvalue(default_value_);
    }


    void Encoder::GDraw(NVGcontext * nvg) {

        nvgBeginPath(nvg);
        nvgRect(nvg, shape->global.c.x, shape->global.c.y, shape->global.s.x, shape->global.s.y);
        nvgStrokeColor(nvg, RED);
        nvgStroke(nvg);
        nvgClosePath(nvg);

        Vec2 wheel_center(shape->global.c.x + shape->global.s.x/2, shape->global.c.y + (0.5f - 0.02f) * shape->global.s.y);
        float wheel_radius = shape->global.s.x * 0.4f;

        nvgBeginPath(nvg);
        nvgCircle(nvg, wheel_center.x, wheel_center.y, wheel_radius);
        nvgFillColor(nvg, GREY);
        nvgFill(nvg);
        nvgClosePath(nvg);

        nvgSave(nvg);
        nvgTranslate(nvg, wheel_center.x, wheel_center.y);
        nvgRotate(nvg, angle);
        nvgBeginPath(nvg);
        nvgRect(nvg, 0, -2, wheel_radius, 4);
        nvgFillColor(nvg, DARK);
        nvgFill(nvg);
        nvgRestore(nvg);
        nvgClosePath(nvg);


        nvgBeginPath(nvg);
        nvgCircle(nvg, wheel_center.x, wheel_center.y, wheel_radius*0.2);
        nvgFillColor(nvg, DARK);
        nvgFill(nvg);
        nvgClosePath(nvg);

        nvgBeginPath(nvg);
        nvgFontSize(nvg, shape->global.s.y * 0.2 * 0.8);
        nvgFontFace(nvg, "sans");
        nvgTextAlign(nvg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

        char buffer[64];
        snprintf(buffer, sizeof buffer, "val: %.2f", value);

        nvgFillColor(nvg, RED);
        nvgText(nvg, shape->global.c.x + shape->global.s.x/2, shape->global.c.y + shape->global.s.y * 0.9f, buffer, NULL);

        nvgFillColor(nvg, GREEN);
        nvgText(nvg, shape->global.c.x + shape->global.s.x/2, shape->global.c.y + shape->global.s.y * 0.1f, label, NULL);
        nvgClosePath(nvg);

        Knob::GDraw(nvg);
    }


    void Encoder::GSetVisible(bool visible_) {
        GObject::GSetVisible(visible_);
//    info_overlay.GSetVisible(false);
    }

    GObject *Encoder::GDragHandler(const Vec2 &v) {
        angle = old_angle + (v.x - drag_from.x) / 100;
        setangle(angle);
        return this;
    }

    GObject *Encoder::GDragBegin(const Vec2 &v) {
        drag_from = v;
        old_angle = angle;
        return this;
    }

}