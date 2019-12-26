//
// Created by Admin on 06.07.2019.
//

#ifndef PD_ENCODER_H
#define PD_ENCODER_H

#include <functional>
#include "Knob.h"
#include "styles.h"
#include "Button.h"

namespace GUI {

    class Encoder;

    class EncoderOverlay : public GObject {

        Encoder * enc;

    public:

        EncoderOverlay(Encoder * enc_) : GObject(BOX) {
            enc = enc_;
        }

        void GDraw(NVGcontext * nvg) override;

    };

    class Encoder : public Knob {

    private:

        friend class EncoderButton;
        friend class EncoderOverlay;

//        const float wheel_radius = 0.4; // of width
//        const float text_height = 0.8; // of space under wheel
//        const float center_wheel_offset = 0.02;
//        const float inner_circle_radius = 0.2;

        float angle = 0;
        float lower_bound, upper_bound;
        float old_angle;
        Vec2 drag_from;

        EncoderOverlay * overlay;

        char * label;

        inline float angle2value(float angle_) {
            return ((angle_ / (float) M_PI + 0.5f) / 0.75f + 1) / 2 * (upper_bound - lower_bound) +
                   lower_bound;
        }

        inline float value2angle(float value_) {
            return (((value_ - lower_bound) / (upper_bound - lower_bound) * 2 - 1) * 0.75f - 0.5f) *
                   (float) M_PI;
        }

        inline void setangle(float angle_) {
            if (angle_ < -1.25f * (float) M_PI) {
                angle_ = -1.25f * (float) M_PI;
            }

            if (angle_ > 0.25f * (float) M_PI) {
                angle_ = 0.25f * (float) M_PI;
            }

            angle = angle_;
            value = angle2value(angle_);
            callback(value);
        }

        std::function<void(float)> callback;

        float value;

//    GCanvas * wheel;

    public:

        Encoder(const char *label_, float default_value_);

        Encoder(const char *label_, float default_value_, unsigned int default_map_);

        Encoder(const char *label_, float default_value_, std::function<void(float)> callback_);

        Encoder(const char *label_, float default_value_, float lower_bound_, float upper_bound_);

        Encoder(const char *label_, float default_value_, std::function<void(float)> callback_,
                unsigned int default_map_);

        Encoder(const char *label_, float default_value_, unsigned int default_map_, float lower_bound_,
                float upper_bound_);

        Encoder(const char *label_, float default_value_, std::function<void(float)> callback_,
                float lower_bound_, float upper_bound_);

        Encoder(const char *label_, float default_value_, std::function<void(float)> callback_,
                unsigned int default_map_, float lower_bound_, float upper_bound_);

        inline void MIn(MData cmd) override {
            Knob::MIn(cmd);
            if (cmd.status == 0xB0 && keymap && cmd.data1 == keymap) {
                setvalue(cmd.data2 / 127.f * (upper_bound - lower_bound) + lower_bound);
            }
        }

        inline void setvalue(float value_) {
            if (value_ < lower_bound) value_ = lower_bound;
            if (value_ > upper_bound) value_ = upper_bound;
            value = value_;
            angle = value2angle(value);
            callback(value);
        }

        Encoder &operator=(const float &value_) {
            setvalue(value_);
            return *this;
        }

        Encoder &operator=(const double &value_) {
            setvalue(value_);
            return *this;
        }

        Encoder &operator=(const int &value_) {
            setvalue(value_);
            return *this;
        }

        operator float() const { return value; }

        void GSetVisible(bool visible_) override;

        virtual void GDraw(NVGcontext * nvg) override;

        GObject *GDragHandler(const Vec2 &v) override;

        GObject *GDragBegin(const Vec2 &v) override;

        GObject *GDragEnd(const Vec2 &v) override;
    };

}

#endif //PD_ENCODER_H
