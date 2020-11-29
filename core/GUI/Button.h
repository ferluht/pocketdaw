//
// Created by Admin on 06.07.2019.
//

#ifndef PD_BUTTON_H
#define PD_BUTTON_H

#include <functional>
#include "Knob.h"
#include "styles.h"

namespace GUI {

    class Button : public Knob {

        friend class EncoderButton;
        friend class LooperButton;

        bool lighted = false;
        bool flashing = false;

        int flashing_counter = 0;
        const int flashing_divider = 40;

        NVGcolor lightColor;
        NVGcolor nextColor;

    public:

        const float textwidth = 0.86;
        const float textheight = 0.8;

        char * labelOn, * labelOff;

        bool state;
        std::function<void(bool)> callback;

        Button(const char *label_, std::function<void(bool)> callback_);
        Button(const char *labelOn_, const char *labelOff_, std::function<void(bool)> callback_);

        operator bool() const { return state; }

        Button &operator=(const bool &state_) {
            state = state_;
            callback(state);
            return *this;
        }

        inline void lightOn(NVGcolor color, float opacity=1.0) {
            lightColor = color;
            lightColor.a *= opacity;
            lighted = true;
        }

        inline void lightOff() {
            lighted = false;
        }

        void GDraw(NVGcontext * nvg) override;

        void GSetVisible(bool visible_) override {
            Knob::GSetVisible(visible_);
        }

        void setFlashing(bool flashing_, NVGcolor nextColor_=MIDGREY) {
            flashing = flashing_;
            nextColor = nextColor_;
        }
    };

    class TapButton : public Button {
    public:
        TapButton(const char * label, std::function<void(bool)> callback_) :
                Button(label, callback_) {
            GSetTapEndCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
                callback(true);
                return nullptr;
            });
        }
    };

    class ProgressButton : public Button {

        float percent;

    public:

        ProgressButton(char *label, std::function<void(bool)> callback_) :
                Button(label, callback_) {
            percent = 0;
        }

        void progress(float percent_) {
            percent = percent_;
        }

        void GDraw(NVGcontext * nvg) override;

    };

    class TexturedMultiButton : public Knob {

        const float textwidth = 0.86;
        const float textheight = 0.8;

        const char **textures;
        unsigned int num_states;

    public:

        unsigned int state;
        std::function<void(unsigned int)> callback;

        TexturedMultiButton(unsigned int num_states_, const char **textures_);

        TexturedMultiButton(std::function<void(unsigned int)> callback_, unsigned int num_states_,
                            const char **textures_);

        GObject *TapEnd(const vecmath::Vec2 &v);

        operator unsigned int() const { return state; }

        TexturedMultiButton &operator=(const unsigned int &state_) {
            state = state_;
            callback(state);
            return *this;
        }

        void GSetVisible(bool visible_) override {
            Knob::GSetVisible(visible_);
        }
    };

    class FocusButton : public Button {

        GObject * focus_object;

    public:

        FocusButton(const char *label, std::function<void(bool)> callback_, GObject * focus_object_) :
                Button(label, callback_) {
            focus_object = focus_object_;
            GSetTapEndCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
                state = !state;
                callback(state);
                if (state)
                    return focus_object;
                return nullptr;
            });
        }

        virtual void GLoseFocus() override {
            Button::GLoseFocus();
            state = false;
        }

    };

    class ListButton : public Knob {

        NVGcolor lightColor;

    public:

        const float textwidth = 0.86;
        const float textheight = 0.8;

        const char ** labels;

        int state;
        std::function<void(int)> callback;

        ListButton(const char ** labels_, std::function<void(int)> callback_);

        operator int() const { return state; }

        void GDraw(NVGcontext * nvg) override;

//        void GSetVisible(bool visible_) override {
//            Knob::GSetVisible(visible_);
//        }
    };

//    class ValueButton : public Button {
//
//        float value;
//
//    public:
//
//        ValueButton(char *label, std::function<void(bool)> callback_) :
//                Button(label, callback_) {
//            value = 0;
//        }
//
//        virtual GObject *GTapEnd(const vecmath::Vec2 &v) override;
//
//        void GDraw(NVGcontext * nvg) override;
//    };

    class LooperButton : public Button {

        float position;

        NVGcolor arc_color;
        NVGcolor body_color;

    public:

        LooperButton(char *label, std::function<void(bool)> callback_) :
                Button(label, callback_) {
            position = 0;
            GSetRatio(1);
        }

        void setArcColor(NVGcolor c){
            arc_color = c;
        }

        void setBodyColor(NVGcolor c){
            body_color = c;
        }

        void setPosition(float position_){
            position = position_ + 1e-10f;
        }

        void GDraw(NVGcontext * nvg) override;

    };

    class XYButton;

    class XYButtonOverlay : public GObject {

        XYButton * btn;

    public:

        XYButtonOverlay(XYButton * btn_) : GObject(BOX) {
            btn = btn_;
        }

        void GDraw(NVGcontext * nvg) override;

    };

    class XYButton : public Button {

        friend class XYButtonOverlay;

        XYButtonOverlay * overlay;

        std::function<void(float, float)> callback;

        char xlabel[50], ylabel[50];
        float xmin, ymin, xrange, yrange;

        bool mem_state;

        float xval = 0, yval = 0;

        int xscale, yscale;

    public:

        enum SCALE {
            LIN,
            EXP
        };

        XYButton(char *label,
                 char *xlabel_, float xmin_, float xmax_,
                 char *ylabel_, float ymin_, float ymax_,
                 std::function<void(float, float)> callback_,
                 int xscale_=LIN, int yscale_=LIN) :
                Button(label, [](bool state){}) {

            callback = callback_;

            overlay = new XYButtonOverlay(this);
            GAttach(overlay);
            overlay->GSetVisible(false);

            sprintf(xlabel, "%s", xlabel_);
            sprintf(ylabel, "%s", ylabel_);

            xscale = xscale_;
            yscale = yscale_;

            xmin = xmin_;
            xrange = xmax_ - xmin_;
//            if (xscale == EXP) xrange = log(xrange);

            ymin = ymin_;
            yrange = ymax_ - ymin_;
//            if (yscale == EXP) yrange = log(yrange);

            GSetDragBeginCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
                mem_state = state;
                state = true;
                overlay->GSetVisible(true);
                return this;
            });

            GSetDragHandlerCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {

                if (xscale == EXP) {
                    xval = pow((v.x / GEngine::screen_width), 4) * xrange + xmin;
                } else {
                    xval = (v.x / GEngine::screen_width) * xrange + xmin;
                }

                if (yscale == EXP) {
                    yval = pow((1 - v.y / GEngine::screen_height), 4) * yrange + ymin;
                } else {
                    yval = (1 - v.y / GEngine::screen_height) * yrange + ymin;
                }

                callback(xval, yval);
//                angle = old_angle + (v.x - drag_from.x) / 100;
                return this;
            });

            GSetDragEndCallback([this](const vecmath::Vec2& v) -> GUI::GObject * {
                state = mem_state;
                overlay->GSetVisible(false);
                return this;
            });
        }

    };

}

#endif //PD_BUTTON_H
