//
// Created by Admin on 06.07.2019.
//

#include "Encoder.h"

namespace GUI {

    const float Encoder::RATIO = 0.92;

    void EncoderOverlay::GDraw(NVGcontext *nvg) {
        nvgBeginPath(nvg);
        nvgRect(nvg, 0, 0, GEngine::screen_width, GEngine::screen_height);
        auto fc = DARKER;
        fc.a = 0.2;
        nvgFillColor(nvg, fc);
        nvgFill(nvg);
        nvgClosePath(nvg);

        nvgBeginPath(nvg);
        nvgFontSize(nvg, GEngine::screen_height * 0.2f);
        nvgFontFace(nvg, "sans");
        nvgTextAlign(nvg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

        char buffer[64];
        snprintf(buffer, sizeof buffer, "%s: %.2f", enc->label, enc->value);

        nvgFillColor(nvg, WHITE);

        nvgText(nvg, GEngine::screen_width / 2, GEngine::screen_height / 2, buffer, NULL);
    }

    Encoder::Encoder(const char *label_, float default_value_, float lower_bound_, float upper_bound_,
            std::function<void(float)> callback_, unsigned int default_map_, unsigned int shape_type_)
            : Knob(shape_type_) {
        GSetRatio(RATIO);
        callback = std::move(callback_);
        lower_bound = lower_bound_;
        upper_bound = upper_bound_;
        range = upper_bound_ - lower_bound_;

        size_t len = strlen(label_);
        label = new char[len + 1];
        strncpy(label, label_, len);

        keymap = default_map_;
        setvalue(default_value_);

        overlay = new EncoderOverlay(this);
        GAttach(overlay);
        overlay->GSetVisible(false);
    }


    void Encoder::GDraw(NVGcontext * nvg) {

        nvgBeginPath(nvg);
        nvgRect(nvg, global.c.x, global.c.y, global.s.x, global.s.y);
        nvgStrokeColor(nvg, RED);
        nvgStroke(nvg);
        nvgClosePath(nvg);

        Vec2 wc(global.c.x + global.s.x * wheel_center.x, global.c.y + global.s.y * wheel_center.y);
        float wr = global.s.x * wheel_radius;

        nvgBeginPath(nvg);
        nvgCircle(nvg, wc.x, wc.y, wr);
        nvgFillColor(nvg, GREY);
        nvgFill(nvg);
        nvgClosePath(nvg);

        nvgSave(nvg);
        nvgTranslate(nvg, wc.x, wc.y);
        nvgRotate(nvg, angle);
        nvgBeginPath(nvg);
        nvgRect(nvg, 0, -2, wr, 4);
        nvgFillColor(nvg, DARK);
        nvgFill(nvg);
        nvgRestore(nvg);
        nvgClosePath(nvg);


        nvgBeginPath(nvg);
        nvgCircle(nvg, wc.x, wc.y, wr*inner_circle_radius);
        nvgFillColor(nvg, DARK);
        nvgFill(nvg);
        nvgClosePath(nvg);

        nvgBeginPath(nvg);
        nvgFontSize(nvg, global.s.y * text_height);
        nvgFontFace(nvg, "sans");
        nvgTextAlign(nvg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

        char buffer[64];
        snprintf(buffer, sizeof buffer, "%s: %.2f", label, value);
        buffer[text_max_length] = 0;

        nvgFillColor(nvg, GREEN);
        nvgText(nvg, global.c.x + global.s.x * text_position.x, global.c.y + global.s.y * text_position.y, buffer, NULL);
        nvgClosePath(nvg);

        Knob::GDraw(nvg);
    }


    void Encoder::GSetVisible(bool visible_) {
        GObject::GSetVisible(visible_);
        overlay->GSetVisible(false);
    }

    GObject *Encoder::GDragEnd(const Vec2 &v) {
        overlay->GSetVisible(false);
        Knob::GDragEnd(v);
        return this;
    }

    GObject *Encoder::GDragHandler(const Vec2 &v) {
        overlay->GSetVisible(true);
        angle = old_angle + (v.x - drag_from.x) / 100;
        setangle(angle);
        return this;
    }

    GObject *Encoder::GDragBegin(const Vec2 &v) {
        drag_from = v;
        old_angle = angle;
        Knob::GDragBegin(v);
        return this;
    }

    void ModulatedEncoder::GDraw(NVGcontext * nvg) {
        Vec2 ac(global.c.x + global.s.x * wheel_center.x, global.c.y + global.s.y * wheel_center.y);
        float ar = global.s.x * wheel_radius;

        nvgBeginPath(nvg);
        float a_from = value2angle(base_value - range * mod_depth);
        float a_to = value2angle(base_value + range * mod_depth);
        nvgArc(nvg, ac.x, ac.y, ar, a_from, a_to, NVG_CW);
        nvgStrokeWidth(nvg, global.s.x * 0.09f);
        nvgStrokeColor(nvg, BLUE);
        nvgStroke(nvg);
        nvgClosePath(nvg);
        nvgStrokeWidth(nvg, 1);

        Encoder::GDraw(nvg);
    }

    GObject *ModulatedEncoder::GDragHandler(const Vec2 &v) {
        Encoder::GDragHandler(v);
        base_value = (Encoder)(*this);
        mod_depth = old_mod_depth - (v.y - drag_from.y) / 1000;
        if (mod_depth > 1) mod_depth = 1;
        if (mod_depth < 0) mod_depth = 0.01;
        return this;
    }

    GObject *ModulatedEncoder::GDragBegin(const Vec2 &v) {
        Encoder::GDragBegin(v);
        dragging = true;
        old_mod_depth = mod_depth;
        return this;
    }

    GObject *ModulatedEncoder::GDragEnd(const Vec2 &v) {
        Encoder::GDragEnd(v);
        dragging = false;
        return this;
    }
}