//
// Created by ibelikov on 24.12.19.
//

#ifndef PD_ENCODERBUTTON_H
#define PD_ENCODERBUTTON_H

#include "Button.h"
#include "Encoder.h"

namespace GUI {

    class EncoderButton : public Encoder {

        void drawLed(NVGcontext *nvg);
        void drawButton(NVGcontext *nvg);
        void drawArc(NVGcontext *nvg);

        bool lighted = false;
        NVGcolor lightColor;
        bool state;

    public:

        EncoderButton(const char * label_, float default_value_=0, float lower_bound_=-1,
                      float upper_bound_=1, unsigned int default_map_=0,
                      std::function<void(float)> drag_callback_=[](float a){},
                      std::function<void(bool)> tap_callback_=[](bool a){})
                : Encoder(label_, default_value_, lower_bound_, upper_bound_, drag_callback_, default_map_, CIRCLE) {
            state = false;

            GSetTapEndCallback([this](const Vec2& v) -> GUI::GObject * {
                state = !state;
                return nullptr;
            });
        }

        void GDraw(NVGcontext * nvg) override;

        inline void lightOn(NVGcolor color, float opacity=1.0) {
            lightColor = color;
            lightColor.a *= opacity;
            lighted = true;
        }

        inline void lightOff() {
            lighted = false;
        }

        operator bool() { return state; }
    };

}

#endif //PD_ENCODERBUTTON_H