//
// Created by Admin on 06.07.2019.
//

#include "Button.h"

namespace GUI {

    Button::Button(const char *label_, std::function<void(bool)> callback_) {
        setShapeType(BOX);
        callback = callback_;
        state = false;

        size_t len = strlen(label_);
        label = new char[len + 1];
        strncpy(label, label_, len);
        label[len] = 0;
    }

    GObject *Button::GTapEnd(const ndk_helper::Vec2 &v) {
        state = !state;
        callback(state);
        return nullptr;
    }


    TexturedMultiButton::TexturedMultiButton(unsigned int num_states_, const char **textures_)
            : TexturedMultiButton([](unsigned int state) {}, num_states_, textures_) {}

    TexturedMultiButton::TexturedMultiButton(std::function<void(unsigned int)> callback_,
                                             unsigned int num_states_, const char **textures_) {
        callback = callback_;
        state = 0;
        num_states = num_states_;
        textures = textures_;
    }

    GObject *TexturedMultiButton::GTapEnd(const ndk_helper::Vec2 &v) {
        state = (state + 1) % num_states;
        callback(state);
        return nullptr;
    }

    void Button::GDraw(NVGcontext * nvg) {
        nvgBeginPath(nvg);
        nvgRect(nvg,
                shape->global.c.x,
                shape->global.c.y,
                shape->global.s.x, shape->global.s.y);
        if (state) {
            nvgFillColor(nvg, YELLOW);
        } else {
            nvgFillColor(nvg, GREY);
        }
        nvgFill(nvg);

        nvgStrokeColor(nvg, BLACK);
        nvgStroke(nvg);

        nvgFontSize(nvg, shape->global.s.y * 0.7);
        nvgFontFace(nvg, "sans");
        nvgTextAlign(nvg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

        nvgFillColor(nvg, BLACK);
        nvgText(nvg, shape->global.c.x + shape->global.s.x/2, shape->global.c.y + shape->global.s.y/2, label, NULL);
        nvgClosePath(nvg);
    }

    void ProgressButton::GDraw(NVGcontext *nvg) {
        nvgBeginPath(nvg);
        nvgRect(nvg,
                shape->global.c.x,
                shape->global.c.y,
                shape->global.s.x, shape->global.s.y);
        nvgFillColor(nvg, GREY);
        nvgFill(nvg);

        nvgStrokeColor(nvg, BLACK);
        nvgStroke(nvg);

        nvgBeginPath(nvg);
        nvgRect(nvg,
                shape->global.c.x,
                shape->global.c.y + 0.02f,
                shape->global.s.x * percent, shape->global.s.y * 0.96f);
        nvgFillColor(nvg, YELLOW);
        nvgFill(nvg);

        nvgFontSize(nvg, shape->global.s.y * 0.7);
        nvgFontFace(nvg, "sans");
        nvgTextAlign(nvg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

        nvgFillColor(nvg, BLACK);
        nvgText(nvg, shape->global.c.x + shape->global.s.x/2, shape->global.c.y + shape->global.s.y/2, label, NULL);
        nvgClosePath(nvg);
    }

}