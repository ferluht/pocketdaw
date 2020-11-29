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

        GSetTapEndCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
            state = !state;
            callback(state);
            return nullptr;
        });
    }

    void Button::GDraw(NVGcontext * nvg) {
        nvgBeginPath(nvg);
        nvgRect(nvg, global.c.x, global.c.y, global.s.x, global.s.y);
        if (state) {
            nvgFillColor(nvg, GEngine::ui_theme->BUTTON_ON_COLOR);
        } else {
            nvgFillColor(nvg, GEngine::ui_theme->BUTTON_OFF_COLOR);
        }
        nvgFill(nvg);
        if (lighted) {
            nvgFillColor(nvg, lightColor);
            nvgFill(nvg);
        }

//        if (state) {
//            nvgFillColor(nvg, GEngine::ui_theme->BUTTON_OUTLINE_ON_COLOR);
//        } else {
//            nvgFillColor(nvg, GEngine::ui_theme->BUTTON_OUTLINE_OFF_COLOR);
//        }
//        nvgStroke(nvg);

        nvgFontSize(nvg, global.s.y * 0.7);
//        if ((global.s.y * strlen(labelOn) > global.s.x) ||
//            (global.s.y * strlen(labelOff) > global.s.x)){
//            nvgFontSize(nvg, global.s.x / strlen(labelOn) / 2);
//        }
        nvgFontFace(nvg, "sans");
        nvgTextAlign(nvg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

        if (state) {
            nvgFillColor(nvg, GEngine::ui_theme->BUTTON_TEXT_ON_COLOR);
        } else {
            nvgFillColor(nvg, GEngine::ui_theme->BUTTON_TEXT_OFF_COLOR);
        }
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

        GSetTapEndCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
            state = (state + 1) % num_states;
            callback(state);
            return nullptr;
        });
    }

    void ProgressButton::GDraw(NVGcontext *nvg) {
        nvgBeginPath(nvg);
        nvgRect(nvg, global.c.x, global.c.y, global.s.x, global.s.y);
        if (state) {
            nvgFillColor(nvg, GEngine::ui_theme->BUTTON_ON_COLOR);
        } else {
            nvgFillColor(nvg, GEngine::ui_theme->BUTTON_OFF_COLOR);
        }
        nvgFill(nvg);

//        if (state) {
//            nvgFillColor(nvg, GEngine::ui_theme->BUTTON_OUTLINE_ON_COLOR);
//        } else {
//            nvgFillColor(nvg, GEngine::ui_theme->BUTTON_OUTLINE_OFF_COLOR);
//        }
//        nvgStroke(nvg);

        nvgBeginPath(nvg);
        nvgRect(nvg, global.c.x, global.c.y + 0.02f, global.s.x * percent, global.s.y * 0.96f);
        nvgFillColor(nvg, GEngine::ui_theme->PROGRESS_BUTTON_BODY_COLOR);
        nvgFill(nvg);

        nvgFontSize(nvg, global.s.y * 0.7);
        nvgFontFace(nvg, "sans");
        nvgTextAlign(nvg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

        char * label = labelOff;
        if (state) label = labelOn;
        if (state) {
            nvgFillColor(nvg, GEngine::ui_theme->BUTTON_TEXT_ON_COLOR);
        } else {
            nvgFillColor(nvg, GEngine::ui_theme->BUTTON_TEXT_OFF_COLOR);
        }
        nvgText(nvg, global.c.x + global.s.x/2, global.c.y + global.s.y/2, label, NULL);
        nvgClosePath(nvg);
    }

    ListButton::ListButton(const char ** labels_, std::function<void(int)> callback_) : Knob(BOX){
//        memcpy(labels, labels_, sizeof (char) * 1 * 4);
        state = 0;
        callback = callback_;

        GSetTapEndCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
            state = (state + 1) % 4;
            callback(state);
            return nullptr;
        });
    }

    void ListButton::GDraw(NVGcontext *nvg) {
        nvgBeginPath(nvg);
        nvgRect(nvg, global.c.x, global.c.y, global.s.x, global.s.y);
        nvgFillColor(nvg, GREY);
        nvgFill(nvg);

        nvgStrokeColor(nvg, BLACK);
        nvgStroke(nvg);

        nvgFontSize(nvg, global.s.y * 0.7);
        nvgFontFace(nvg, "sans");
        nvgTextAlign(nvg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

        nvgFillColor(nvg, BLACK);
        std::string s = std::to_string(state);
        char const *label = s.c_str();
        nvgText(nvg, global.c.x + global.s.x/2, global.c.y + global.s.y/2, label, NULL);
        nvgClosePath(nvg);
    }

    void LooperButton::GDraw(NVGcontext *nvg) {
        nvgBeginPath(nvg);

        vecmath::Vec2 button_center(global.c.x + global.s.x/2, global.c.y + global.s.y/2);
        float button_radius = global.s.x * 0.4f;

        nvgBeginPath(nvg);
        nvgArc(nvg, button_center.x, button_center.y, button_radius, 2 * M_PI * position + 0.5 * M_PI, 0.5 * M_PI, NVG_CCW);
        nvgStrokeWidth(nvg, global.s.x * 0.1f);
        nvgStrokeColor(nvg, arc_color);
        nvgStroke(nvg);
        nvgClosePath(nvg);
        nvgStrokeWidth(nvg, 1);

        nvgBeginPath(nvg);
        nvgCircle(nvg, button_center.x, button_center.y, button_radius * 0.98f);
        if (state) {
            if (flashing) {
                flashing_counter = (flashing_counter + 1) % flashing_divider;
                if (flashing_counter > flashing_divider / 2) {
                    nvgFillColor(nvg, body_color);
                } else {
                    nvgFillColor(nvg, nextColor);
                }
            } else {
                nvgFillColor(nvg, body_color);
            }
        } else {
            nvgFillColor(nvg, MIDGREY);
        }
        nvgFill(nvg);
        nvgClosePath(nvg);

        nvgClosePath(nvg);
    }

    void XYButtonOverlay::GDraw(NVGcontext *nvg) {
        nvgBeginPath(nvg);
        nvgRect(nvg, 0, 0, GEngine::screen_width, GEngine::screen_height);
        auto fc = DARKER;
        fc.a = 0.2;
        nvgFillColor(nvg, fc);
        nvgFill(nvg);
        nvgClosePath(nvg);

        nvgBeginPath(nvg);
        nvgFontSize(nvg, GEngine::screen_height * 0.1f);
        nvgFontFace(nvg, "sans");
        nvgTextAlign(nvg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

        char buffer[100];
        snprintf(buffer, sizeof buffer, "%s: %.2f   %s: %.2f", btn->xlabel, btn->xval, btn->ylabel, btn->yval);

        nvgFillColor(nvg, WHITE);

        nvgText(nvg, GEngine::screen_width / 2, GEngine::screen_height / 2, buffer, NULL);
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