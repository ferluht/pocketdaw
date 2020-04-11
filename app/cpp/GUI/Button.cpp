//
// Created by Admin on 06.07.2019.
//

#include "Button.h"

namespace GUI {

    Button::Button(const char *label_, std::function<void(bool)> callback_) :
    Button(label_, label_, callback_){}

    Button::Button(const char *labelOn_, const char *labelOff_, std::function<void(bool)> callback_) : Knob(BOX) {
        callback = callback_;
        state = false;

        size_t len = strlen(labelOn_);
        labelOn = new char[len + 1];
        strncpy(labelOn, labelOn_, len);
        labelOn[len] = 0;

        if (labelOn_ == labelOff_) {
            labelOff = labelOn;
        } else {
            len = strlen(labelOff_);
            labelOff = new char[len + 1];
            strncpy(labelOff, labelOff_, len);
            labelOff[len] = 0;
        }

        GSetTapEndCallback([this](const Vec2& v) -> GUI::GObject * {
            state = !state;
            callback(state);
            return nullptr;
        });
    }

    void Button::GDraw(NVGcontext * nvg) {
        nvgBeginPath(nvg);
        nvgRect(nvg, global.c.x, global.c.y, global.s.x, global.s.y);
        if (state) {
            nvgFillColor(nvg, YELLOW);
        } else {
            nvgFillColor(nvg, GREY);
        }
        nvgFill(nvg);
        if (lighted) {
            nvgFillColor(nvg, lightColor);
            nvgFill(nvg);
        }

        nvgStrokeColor(nvg, BLACK);
        nvgStroke(nvg);

        nvgFontSize(nvg, global.s.y * 0.7);
        nvgFontFace(nvg, "sans");
        nvgTextAlign(nvg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

        nvgFillColor(nvg, BLACK);
        char * label = labelOff;
        if (state) label = labelOn;
        nvgText(nvg, global.c.x + global.s.x/2, global.c.y + global.s.y/2, label, NULL);
        nvgClosePath(nvg);
    }

    TexturedMultiButton::TexturedMultiButton(unsigned int num_states_, const char **textures_)
            : TexturedMultiButton([](unsigned int state) {}, num_states_, textures_) {}

    TexturedMultiButton::TexturedMultiButton(std::function<void(unsigned int)> callback_,
                                             unsigned int num_states_, const char **textures_) : Knob(BOX) {
        callback = callback_;
        state = 0;
        num_states = num_states_;
        textures = textures_;

        GSetTapEndCallback([this](const Vec2& v) -> GUI::GObject * {
            state = (state + 1) % num_states;
            callback(state);
            return nullptr;
        });
    }

    void ProgressButton::GDraw(NVGcontext *nvg) {
        nvgBeginPath(nvg);
        nvgRect(nvg, global.c.x, global.c.y, global.s.x, global.s.y);
        nvgFillColor(nvg, GREY);
        nvgFill(nvg);

        nvgStrokeColor(nvg, BLACK);
        nvgStroke(nvg);

        nvgBeginPath(nvg);
        nvgRect(nvg, global.c.x, global.c.y + 0.02f, global.s.x * percent, global.s.y * 0.96f);
        nvgFillColor(nvg, YELLOW);
        nvgFill(nvg);

        nvgFontSize(nvg, global.s.y * 0.7);
        nvgFontFace(nvg, "sans");
        nvgTextAlign(nvg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

        char * label = labelOff;
        if (state) label = labelOn;
        nvgFillColor(nvg, BLACK);
        nvgText(nvg, global.c.x + global.s.x/2, global.c.y + global.s.y/2, label, NULL);
        nvgClosePath(nvg);
    }

//    void ValueButton::GDraw(NVGcontext *nvg) {
//        nvgBeginPath(nvg);
//        nvgRect(nvg,
//                shape->global.c.x,
//                shape->global.c.y,
//                shape->global.s.x, shape->global.s.y);
//        nvgFillColor(nvg, GREY);
//        nvgFill(nvg);
//
//        nvgStrokeColor(nvg, BLACK);
//        nvgStroke(nvg);
//
//        nvgBeginPath(nvg);
//        nvgRect(nvg,
//                shape->global.c.x,
//                shape->global.c.y + 0.02f,
//                shape->global.s.x * 0.96f, shape->global.s.y * value);
//        nvgFillColor(nvg, YELLOW);
//        nvgFill(nvg);
//
//        nvgFontSize(nvg, shape->global.s.y * 0.7);
//        nvgFontFace(nvg, "sans");
//        nvgTextAlign(nvg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
//
//        char * label = labelOff;
//        if (state) label = labelOn;
//        nvgFillColor(nvg, BLACK);
//        nvgText(nvg, shape->global.c.x + shape->global.s.x/2, shape->global.c.y + shape->global.s.y/2, label, NULL);
//        nvgClosePath(nvg);
//    }

}